# 2000+   动态库编译Makefile
# Makefile for armBirmTest project - Full Integration Version
# 用blas-ft实现
# ============================================================================
# 1. 基础配置
# ============================================================================
PROJECT_NAME = armBirmTest

# 目录定义
SRC_DIR   = ./src
BUILD_DIR = ./build
BIN_DIR   = ./bin
LIB_DIR   = ./lib

# 编译器路径 (Phytium GCC)
#CC = /opt/compiler/phygcc-10.3.3-20240926-aarch64-linux/bin/gcc
CC = gcc

# ============================================================================
# 2. 编译选项 (CFLAGS)
# ============================================================================

# 架构优化 (针对飞腾 S5000C)
#ARCH_FLAGS = -march=armv8.2-a+simd+crc+crypto -mtune=ftc86x
# 架构优化 (针对飞腾 2000+/64)
ARCH_FLAGS = -march=armv8.2-a+simd -mtune=ftc66x
#ARCH_FLAGS = -march=armv8-a -mtune=cortex-a53

# 优化等级 (默认 O3)
OPT_LEVEL  = -O3 -fopt-info-vec-missed
# 不优化
#OPT_LEVEL  = -O0

# 警告与标准
WARN_FLAGS = -Wall -Wextra -Wno-unused-parameter
STD_FLAGS  = -std=gnu11

# 头文件搜索路径 (-I)
# 注意：将所有库的 include 目录都加在这里
INCLUDE_DIRS = -I./src/common \
               -I./src/birm \
               -I./src/armPublic2/slog \
               -I./src/test \
               -I./src/armPublic2/etime \
               -I/opt/arm/armpl_25.07_gcc/include \

# 汇总 CFLAGS
CFLAGS = $(ARCH_FLAGS) $(OPT_LEVEL) $(WARN_FLAGS) $(STD_FLAGS) \
         $(INCLUDE_DIRS)  \
         -ffast-math -funroll-loops -ftree-vectorize -fprefetch-loop-arrays -flto -faggressive-loop-optimizations
# 汇总 CFLAGS (无强制附加优化)
#CFLAGS = $(ARCH_FLAGS) $(OPT_LEVEL) $(WARN_FLAGS) $(STD_FLAGS) \
#         $(INCLUDE_DIRS)

# ============================================================================
# 3. 链接选项 (LDFLAGS)
# ============================================================================

# 库文件搜索路径 (-L)
LIBRARY_PATHS = -L/opt/arm/armpl_25.07_gcc/lib \
                -L$(SRC_DIR)/armPublic2/slog \
                -L$(SRC_DIR)/armPublic2/etime \
                -L$(LIB_DIR) \

# 需要链接的库 (-l)
# 链接顺序说明：
# 1. VSIPL (高层库)
# 2. BLAS / FFTW (计算库)
# 3. VML / SML (基础数学库)
# 4. 系统库 (lm, lpthread)
LIBS = -larmpl_lp64 -lamath -lastring \
       -lslog \
       -letime \
       -static-libgfortran -static-libgcc \
       -lm -lpthread -fopenmp

# 汇总 LDFLAGS
LDFLAGS = $(LIBRARY_PATHS) $(LIBS) -Wl,--gc-sections

# ============================================================================
# 4. 文件搜索与目标定义
# ============================================================================

# 递归查找所有 .c 和 .S 文件
SRCS := $(shell find $(SRC_DIR) -name '*.c' -type f) \
        $(shell find $(SRC_DIR) -name '*.S' -type f)

# 生成对应的 .o 文件路径
OBJS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(filter %.c,$(SRCS))) \
        $(patsubst $(SRC_DIR)/%.S,$(BUILD_DIR)/%.o,$(filter %.S,$(SRCS)))

# 自动生成依赖文件 (.d)
DEPS := $(OBJS:.o=.d)

# 最终目标文件
TARGET = $(BIN_DIR)/$(PROJECT_NAME)

# ============================================================================
# 5. 构建规则
# ============================================================================

.PHONY: all clean debug release run info

# 默认目标
all: $(TARGET)

# 链接规则
$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	@echo "Linking $(PROJECT_NAME)..."
	$(CC) $(CFLAGS) $(OBJS) -o $@ $(LDFLAGS)
	@echo "Build Success: $@"

# C 文件编译规则
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "Compiling $<..."
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# 汇编文件编译规则
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.S
	@mkdir -p $(dir $@)
	@echo "Assembling $<..."
	$(CC) $(CFLAGS) -c $< -o $@

# 引入依赖文件 (用于增量编译)
-include $(DEPS)

# ============================================================================
# 6. 辅助目标
# ============================================================================

# 调试模式 (make debug)
debug: CFLAGS += -g -DDEBUG -O0
debug: all

# (make trace)
trace: OPT_LEVEL  := -O3
trace: CFLAGS += -g -fno-omit-frame-pointer -DDEBUG \
                -fno-ipa-cp -fno-ipa-cp-clone \
                -fno-ipa-pta -fno-tree-pta
trace: LDFLAGS += -Wl,--build-id
trace: all

# 发布模式 (make release) - 去除符号表
release: CFLAGS += -s -DNDEBUG
release: all

# 清理
clean:
	@echo "Cleaning..."
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# 运行
run: $(TARGET)
	@echo "Running..."
	./$(TARGET)

# 显示配置信息 (增加显示新库路径)
info:
	@echo "Compiler:  $(CC)"
	@echo "VML Path:  $(VML_DIR)"
	@echo "SML Path:  $(SML_DIR)"
	@echo "FFTW Path: $(FFTW_DIR)"
	@echo "BLAS Path: $(BLAS_DIR)"
	@echo "VSIP Path: $(VSIPL_DIR)"
	@echo "Sources:   $(words $(SRCS)) files"
	@echo "Output:    $(TARGET)"
