bool trans_MULW(DisasContext *ctx, arg_MULW *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    LOAD_ARGS
    tcg_gen_mul_tl(source1, source1, source2);
    tcg_gen_ext32s_tl(source1, source1);
    RESULT_AND_FREE
    return true;
#else
    return false;
#endif
}

static bool trans_DIVW(DisasContext *ctx, arg_DIVW *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    LOAD_ARGS
    tcg_gen_ext32s_tl(source1, source1);
    tcg_gen_ext32s_tl(source2, source2);
    gen_div(source1, source1, source2);
    tcg_gen_ext32s_tl(source1, source1);
    RESULT_AND_FREE
    return true;
#else
    return false;
#endif
}

static bool trans_DIVUW(DisasContext *ctx, arg_DIVUW *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    LOAD_ARGS
    tcg_gen_ext32u_tl(source1, source1);
    tcg_gen_ext32u_tl(source2, source2);
    gen_divu(source1, source1, source2);
    tcg_gen_ext32s_tl(source1, source1);
    RESULT_AND_FREE
    return true;
#else
    return false;
#endif
}

static bool trans_REMW(DisasContext *ctx, arg_REMW *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    LOAD_ARGS
    tcg_gen_ext32s_tl(source1, source1);
    tcg_gen_ext32s_tl(source2, source2);
    gen_rem(source1, source1, source2);
    tcg_gen_ext32s_tl(source1, source1);
    RESULT_AND_FREE
    return true;
#else
    return false;
#endif
}

static bool trans_REMUW(DisasContext *ctx, arg_REMUW *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    LOAD_ARGS
    tcg_gen_ext32u_tl(source1, source1);
    tcg_gen_ext32u_tl(source2, source2);
    gen_remu(source1, source1, source2);
    tcg_gen_ext32s_tl(source1, source1);
    RESULT_AND_FREE
    return true;
#else
    return false;
#endif
}
