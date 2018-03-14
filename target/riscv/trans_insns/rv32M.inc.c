static bool trans_MUL(DisasContext *ctx, arg_MUL *a, uint32_t insn)
{
    LOAD_ARGS
    tcg_gen_mul_tl(source1, source1, source2);
    RESULT_AND_FREE
    return true;
}

static bool trans_MULH(DisasContext *ctx, arg_MULH *a, uint32_t insn)
{
    LOAD_ARGS
    tcg_gen_muls2_tl(source2, source1, source1, source2);
    RESULT_AND_FREE
    return true;
}

static bool trans_MULHSU(DisasContext *ctx, arg_MULHSU *a, uint32_t insn)
{
    LOAD_ARGS
    gen_mulhsu(source1, source1, source2);
    RESULT_AND_FREE
    return true;
}

static bool trans_MULHU(DisasContext *ctx, arg_MULHU *a, uint32_t insn)
{
    LOAD_ARGS
    tcg_gen_mulu2_tl(source2, source1, source1, source2);
    RESULT_AND_FREE
    return true;
}

static bool trans_DIV(DisasContext *ctx, arg_DIV *a, uint32_t insn)
{
    LOAD_ARGS
    gen_div(source1, source1, source2);
    RESULT_AND_FREE
    return true;
}

static bool trans_DIVU(DisasContext *ctx, arg_DIVU *a, uint32_t insn)
{
    LOAD_ARGS
    gen_divu(source1, source1, source2);
    RESULT_AND_FREE
    return true;
}

static bool trans_REM(DisasContext *ctx, arg_REM *a, uint32_t insn)
{
    LOAD_ARGS
    gen_rem(source1, source1, source2);
    RESULT_AND_FREE
    return true;
}

static bool trans_REMU(DisasContext *ctx, arg_REMU *a, uint32_t insn)
{
    LOAD_ARGS
    gen_remu(source1, source1, source2);
    RESULT_AND_FREE
    return true;
}

