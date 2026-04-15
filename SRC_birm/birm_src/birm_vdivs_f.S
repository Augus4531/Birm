/*
 * Function: birm_vdivs_f
 * Optimization: Enhanced for Phytium S5000C (<= 16k points)
 */

.arch armv8-a
.text
.align 6
.global birm_vdivs_f
.type birm_vdivs_f, %function

birm_vdivs_f:
    // ---------------------------------------------------------
    // 0. 参数检查 & 标量准备
    // ---------------------------------------------------------
    // x0=src, s0=scalar, w1=nx, x2=dst
    cbz     x0, .Lerr_null
    cbz     x2, .Lerr_null
    cmp     w1, #0
    ble     .Lerr_len

    // 计算倒数: 1.0 / x2
    fmov    s1, #1.0
    fdiv    s0, s1, s0
    dup     v30.4s, v0.s[0]     // 广播倒数到 v30

    // ---------------------------------------------------------
    // 1. 分支判断
    // ---------------------------------------------------------
    // 保持原阈值逻辑，大于32000走STNP，小于32000走Cache优化
    mov     w3, #32000
    cmp     w1, w3
    b.ge    .Llarge_data_path

    // =========================================================
    // Case 1: 优化后的 Cache 路径 (针对 <= 16k 优化)
    // 优化点：
    // 1. 64路展开 (使用 v0-v7 和 v16-v23)
    // 2. 加入 prfm 预取 (针对 16k=64KB 超过 L1 容量的情况)
    // 3. 指令交错隐藏延迟
    // =========================================================
.Lsmall_data_path:
    // 如果不足 64 个，去处理尾部
    cmp     w1, #64
    b.lt    .Lsmall_tail_handler

    // 核心循环：64 floats (256 bytes)
.Lsmall_loop_64:
    // [优化] 预取: 提前 320 字节
    prfm    pldl1keep, [x0, #320]

    // 1. Load All (利用内存并发能力)
    ld1     {v0.4s, v1.4s, v2.4s, v3.4s}, [x0], #64
    ld1     {v4.4s, v5.4s, v6.4s, v7.4s}, [x0], #64
    ld1     {v16.4s, v17.4s, v18.4s, v19.4s}, [x0], #64
    ld1     {v20.4s, v21.4s, v22.4s, v23.4s}, [x0], #64

    // 2. Calc Block A
    fmul    v0.4s, v0.4s, v30.4s
    fmul    v1.4s, v1.4s, v30.4s
    fmul    v2.4s, v2.4s, v30.4s
    fmul    v3.4s, v3.4s, v30.4s
    fmul    v4.4s, v4.4s, v30.4s
    fmul    v5.4s, v5.4s, v30.4s
    fmul    v6.4s, v6.4s, v30.4s
    fmul    v7.4s, v7.4s, v30.4s

<<<<<<< HEAD
    // 3. Calc Block B
=======
    // 3. Calc Block B (关键改动：将计算提前！)
>>>>>>> 53029a6741c0c8d844a0286fdd73d1b8bccaaaea
    // 在这里执行 Block B 的乘法，填补 Block A 乘法的 Latency 窗口
    fmul    v16.4s, v16.4s, v30.4s
    fmul    v17.4s, v17.4s, v30.4s
    fmul    v18.4s, v18.4s, v30.4s
    fmul    v19.4s, v19.4s, v30.4s
    fmul    v20.4s, v20.4s, v30.4s
    fmul    v21.4s, v21.4s, v30.4s
    fmul    v22.4s, v22.4s, v30.4s
    fmul    v23.4s, v23.4s, v30.4s

<<<<<<< HEAD
    // 4. Store All (此时 Block A 的结果已经准备好了，无需等)
=======
    // 4. Store All (此时 Block A 的结果肯定已经准备好了，无需等待)
>>>>>>> 53029a6741c0c8d844a0286fdd73d1b8bccaaaea
    st1     {v0.4s, v1.4s, v2.4s, v3.4s}, [x2], #64
    st1     {v4.4s, v5.4s, v6.4s, v7.4s}, [x2], #64
    st1     {v16.4s, v17.4s, v18.4s, v19.4s}, [x2], #64
    st1     {v20.4s, v21.4s, v22.4s, v23.4s}, [x2], #64

    sub     w1, w1, #64
    cmp     w1, #64
    b.ge    .Lsmall_loop_64

    // ---------------------------------------------------------
    // 尾部处理 (64个以内) - 复用之前的逻辑，增加对 32-63 区间的检查
    // ---------------------------------------------------------
.Lsmall_tail_handler:
    cbz     w1, .Ldone

    // Check 32 (由于上面是64循环，这里可能剩下 >= 32)
    tbz     w1, #5, .Lsm_check_16
    ld1     {v0.4s, v1.4s, v2.4s, v3.4s}, [x0], #64
    ld1     {v4.4s, v5.4s, v6.4s, v7.4s}, [x0], #64
    fmul    v0.4s, v0.4s, v30.4s
    fmul    v1.4s, v1.4s, v30.4s
    fmul    v2.4s, v2.4s, v30.4s
    fmul    v3.4s, v3.4s, v30.4s
    fmul    v4.4s, v4.4s, v30.4s
    fmul    v5.4s, v5.4s, v30.4s
    fmul    v6.4s, v6.4s, v30.4s
    fmul    v7.4s, v7.4s, v30.4s
    st1     {v0.4s, v1.4s, v2.4s, v3.4s}, [x2], #64
    st1     {v4.4s, v5.4s, v6.4s, v7.4s}, [x2], #64

.Lsm_check_16:
    tbz     w1, #4, .Lsm_check_8
    ld1     {v0.4s, v1.4s, v2.4s, v3.4s}, [x0], #64
    fmul    v0.4s, v0.4s, v30.4s
    fmul    v1.4s, v1.4s, v30.4s
    fmul    v2.4s, v2.4s, v30.4s
    fmul    v3.4s, v3.4s, v30.4s
    st1     {v0.4s, v1.4s, v2.4s, v3.4s}, [x2], #64

.Lsm_check_8:
    tbz     w1, #3, .Lsm_check_4
    ld1     {v0.4s, v1.4s}, [x0], #32
    fmul    v0.4s, v0.4s, v30.4s
    fmul    v1.4s, v1.4s, v30.4s
    st1     {v0.4s, v1.4s}, [x2], #32

.Lsm_check_4:
    tbz     w1, #2, .Lsm_check_2
    ld1     {v0.4s}, [x0], #16
    fmul    v0.4s, v0.4s, v30.4s
    st1     {v0.4s}, [x2], #16

.Lsm_check_2:
    tbz     w1, #1, .Lsm_check_1
    ld1     {v0.2s}, [x0], #8
    fmul    v0.2s, v0.2s, v30.2s
    st1     {v0.2s}, [x2], #8

.Lsm_check_1:
    tbz     w1, #0, .Ldone
    ldr     s0, [x0]
    fmul    s0, s0, v30.s[0]
    str     s0, [x2]
    b       .Ldone

    // =========================================================
<<<<<<< HEAD
    // Case 2: 大数据量 
    // =========================================================
.Llarge_data_path:
=======
    // Case 2: 大数据量 (nx >= 32000) - DDR 带宽极速模式 (保持不变)
    // =========================================================
.Llarge_data_path:
    // ... (保持原有的 STNP 逻辑) ...
>>>>>>> 53029a6741c0c8d844a0286fdd73d1b8bccaaaea
    // 对齐 dst
    tst     x2, #63
    b.eq    .Llarge_loop_32

.Lalign_loop:
    ldr     s1, [x0], #4
    fmul    s1, s1, v30.s[0]
    str     s1, [x2], #4
    sub     w1, w1, #1
    cbz     w1, .Ldone
    tst     x2, #63
    b.ne    .Lalign_loop

    cmp     w1, #32
    b.lt    .Llarge_tail_handler

.Llarge_loop_32:
    prfm    pldl1keep, [x0, #512]
    ld1     {v0.4s, v1.4s, v2.4s, v3.4s}, [x0], #64
    ld1     {v4.4s, v5.4s, v6.4s, v7.4s}, [x0], #64
    fmul    v0.4s, v0.4s, v30.4s
    fmul    v1.4s, v1.4s, v30.4s
    fmul    v2.4s, v2.4s, v30.4s
    fmul    v3.4s, v3.4s, v30.4s
    fmul    v4.4s, v4.4s, v30.4s
    fmul    v5.4s, v5.4s, v30.4s
    fmul    v6.4s, v6.4s, v30.4s
    fmul    v7.4s, v7.4s, v30.4s
    stnp    q0, q1, [x2]
    stnp    q2, q3, [x2, #32]
    stnp    q4, q5, [x2, #64]
    stnp    q6, q7, [x2, #96]
    add     x2, x2, #128
    sub     w1, w1, #32
    cmp     w1, #32
    b.ge    .Llarge_loop_32

.Llarge_tail_handler:
    b       .Lsmall_tail_handler

.Ldone:
    mov     w0, #0
    ret

.Lerr_null:
    mov     w0, #-1
    ret

.Lerr_len:
    mov     w0, #-2
    ret
