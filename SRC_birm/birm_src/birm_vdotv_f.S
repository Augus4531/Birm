/*
 * Function: birm_vdotv_f
 * Architecture: ARMv8-A (AArch64)
 * Optimization: Phytium S5000C (Streaming Load Edition - Fixed)
 * ChangeLog:
 * - FIXED: .Lcheck_32 branch was missing logic for the second half of data (processed 16 instead of 32).
 */

.text
.align 6
.global birm_vdotv_f
.type birm_vdotv_f, %function

// 定义常量
.equ ERR_NULL, -1
.equ ERR_LEN,  -2

birm_vdotv_f:
    // ---------------------------------------------------------
    // 0. 参数检查
    // ---------------------------------------------------------
    cbz     x0, .Lerr_null
    cbz     x1, .Lerr_null
    cbz     x3, .Lerr_null
    cmp     w2, #0
    ble     .Lerr_len

    // ---------------------------------------------------------
    // 1. 初始化累加器 (v16-v23)
    // ---------------------------------------------------------
    eor     v16.16b, v16.16b, v16.16b
    eor     v17.16b, v17.16b, v17.16b
    eor     v18.16b, v18.16b, v18.16b
    eor     v19.16b, v19.16b, v19.16b
    eor     v20.16b, v20.16b, v20.16b
    eor     v21.16b, v21.16b, v21.16b
    eor     v22.16b, v22.16b, v22.16b
    eor     v23.16b, v23.16b, v23.16b

    // ---------------------------------------------------------
    // 2. 检查数据量 (>= 64 floats)
    // ---------------------------------------------------------
    cmp     w2, #64
    b.lt    .Lcheck_32

    // 计算循环次数 (nx / 64)
    lsr     w4, w2, #6

    // ---------------------------------------------------------
    // 3. 核心大循环 (LDNP / No Prefetch / Immediate Offset)
    // ---------------------------------------------------------
    .p2align 6
.Lloop_64:
    // --- Block 1: 0-63 Bytes ---
    ldnp    q0, q1, [x0]
    ldnp    q24, q25, [x1]
    ldnp    q2, q3, [x0, #32]
    ldnp    q26, q27, [x1, #32]

    fmla    v16.4s, v0.4s, v24.4s
    fmla    v17.4s, v1.4s, v25.4s
    fmla    v18.4s, v2.4s, v26.4s
    fmla    v19.4s, v3.4s, v27.4s

    // --- Block 2: 64-127 Bytes ---
    ldnp    q4, q5, [x0, #64]
    ldnp    q28, q29, [x1, #64]
    ldnp    q6, q7, [x0, #96]
    ldnp    q30, q31, [x1, #96]

    fmla    v20.4s, v4.4s, v28.4s
    fmla    v21.4s, v5.4s, v29.4s
    fmla    v22.4s, v6.4s, v30.4s
    fmla    v23.4s, v7.4s, v31.4s

    // --- Block 3: 128-191 Bytes ---
    ldnp    q0, q1, [x0, #128]
    ldnp    q24, q25, [x1, #128]
    ldnp    q2, q3, [x0, #160]
    ldnp    q26, q27, [x1, #160]

    fmla    v16.4s, v0.4s, v24.4s
    fmla    v17.4s, v1.4s, v25.4s
    fmla    v18.4s, v2.4s, v26.4s
    fmla    v19.4s, v3.4s, v27.4s

    // --- Block 4: 192-255 Bytes ---
    ldnp    q4, q5, [x0, #192]
    ldnp    q28, q29, [x1, #192]
    ldnp    q6, q7, [x0, #224]
    ldnp    q30, q31, [x1, #224]

    fmla    v20.4s, v4.4s, v28.4s
    fmla    v21.4s, v5.4s, v29.4s
    fmla    v22.4s, v6.4s, v30.4s
    fmla    v23.4s, v7.4s, v31.4s

    // 更新指针
    add     x0, x0, #256
    add     x1, x1, #256

    subs    w4, w4, #1
    b.ne    .Lloop_64

    // 更新剩余点数
    and     w2, w2, #63

    // ---------------------------------------------------------
    // 4. 处理剩余块 (32/16/4/1) - 使用普通 LDP
    // ---------------------------------------------------------
.Lcheck_32:
    cmp     w2, #32
    b.lt    .Lcheck_16

    // [FIXED] 之前这里只处理了 16 个点，现在补全为 32 个点
    
    // Part A: First 16 floats
    ldp     q0, q1, [x0], #32
    ldp     q24, q25, [x1], #32
    ldp     q2, q3, [x0], #32
    ldp     q26, q27, [x1], #32

    fmla    v16.4s, v0.4s, v24.4s
    fmla    v17.4s, v1.4s, v25.4s
    fmla    v18.4s, v2.4s, v26.4s
    fmla    v19.4s, v3.4s, v27.4s

    // Part B: Next 16 floats (Missing in previous version)
    ldp     q4, q5, [x0], #32
    ldp     q28, q29, [x1], #32
    ldp     q6, q7, [x0], #32
    ldp     q30, q31, [x1], #32

    fmla    v20.4s, v4.4s, v28.4s
    fmla    v21.4s, v5.4s, v29.4s
    fmla    v22.4s, v6.4s, v30.4s
    fmla    v23.4s, v7.4s, v31.4s

    sub     w2, w2, #32

.Lcheck_16:
    cmp     w2, #16
    b.lt    .Lreduce_vectors

    ldp     q0, q1, [x0], #32
    ldp     q24, q25, [x1], #32
    ldp     q2, q3, [x0], #32
    ldp     q26, q27, [x1], #32

    fmla    v16.4s, v0.4s, v24.4s
    fmla    v17.4s, v1.4s, v25.4s
    fmla    v18.4s, v2.4s, v26.4s
    fmla    v19.4s, v3.4s, v27.4s

    sub     w2, w2, #16

    // ---------------------------------------------------------
    // 5. 向量归约 (Reduction)
    // ---------------------------------------------------------
.Lreduce_vectors:
    // 8 -> 4
    fadd    v16.4s, v16.4s, v20.4s
    fadd    v17.4s, v17.4s, v21.4s
    fadd    v18.4s, v18.4s, v22.4s
    fadd    v19.4s, v19.4s, v23.4s

    // 4 -> 2
    fadd    v16.4s, v16.4s, v18.4s
    fadd    v17.4s, v17.4s, v19.4s

    // 2 -> 1
    fadd    v0.4s, v16.4s, v17.4s

    // 向量内求和
    faddp   v0.4s, v0.4s, v0.4s
    faddp   v0.4s, v0.4s, v0.4s
    
    // 保存结果到 s10
    fmov    s10, s0

    // ---------------------------------------------------------
    // 6. 处理剩余 4 点块
    // ---------------------------------------------------------
    cmp     w2, #4
    b.lt    .Lcheck_scalar

.Lloop_4:
    ldr     q0, [x0], #16
    ldr     q1, [x1], #16
    fmul    v0.4s, v0.4s, v1.4s
    faddp   v0.4s, v0.4s, v0.4s
    faddp   v0.4s, v0.4s, v0.4s
    fadd    s10, s10, s0
    sub     w2, w2, #4
    cmp     w2, #4
    b.ge    .Lloop_4

    // ---------------------------------------------------------
    // 7. 处理标量剩余
    // ---------------------------------------------------------
.Lcheck_scalar:
    cbz     w2, .Ldone

.Lloop_scalar:
    ldr     s0, [x0], #4
    ldr     s1, [x1], #4
    fmadd   s10, s0, s1, s10
    sub     w2, w2, #1
    cbnz    w2, .Lloop_scalar

.Ldone:
    str     s10, [x3]
    mov     w0, #0
    ret

.Lerr_null:
    mov     w0, ERR_NULL
    ret

.Lerr_len:
    mov     w0, ERR_LEN
    ret

<<<<<<< HEAD
.size birm_vdotv_f, .-birm_vdotv_f
=======
.size birm_vdotv_f, .-birm_vdotv_f
>>>>>>> 53029a6741c0c8d844a0286fdd73d1b8bccaaaea
