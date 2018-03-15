#define MOP (MO_ALIGN | MO_TESL)
bool trans_LRW(DisasContext *ctx, arg_LRW *a, uint32_t insn)
{
    gen_lrwd(ctx, a->rd, a->rs1, a->rl, a->aq, MOP);
    return true;
}

static bool trans_SCW(DisasContext *ctx, arg_SCW *a, uint32_t insn)
{
    gen_scwd(ctx, a->rd, a->rs1, a->rs2, a->rl, a->aq, MOP);
    return true;
}

static bool trans_AMOSWAPW(DisasContext *ctx, arg_AMOSWAPW *a, uint32_t insn)
{
    LOAD_ARGS
    tcg_gen_atomic_xchg_tl(source2, source1, source2, ctx->mem_idx, MOP);
    RESULT_AND_FREE
    return true;
}

static bool trans_AMOADDW(DisasContext *ctx, arg_AMOADDW *a, uint32_t insn)
{
    LOAD_ARGS
    tcg_gen_atomic_fetch_add_tl(source2, source1, source2, ctx->mem_idx, MOP);
    RESULT_AND_FREE
    return true;
}

static bool trans_AMOXORW(DisasContext *ctx, arg_AMOXORW *a, uint32_t insn)
{
    LOAD_ARGS
    tcg_gen_atomic_fetch_xor_tl(source2, source1, source2, ctx->mem_idx, MOP);
    RESULT_AND_FREE
    return true;
}

static bool trans_AMOANDW(DisasContext *ctx, arg_AMOANDW *a, uint32_t insn)
{
    LOAD_ARGS
    tcg_gen_atomic_fetch_and_tl(source2, source1, source2, ctx->mem_idx, MOP);
    RESULT_AND_FREE
    return true;
}

static bool trans_AMOORW(DisasContext *ctx, arg_AMOORW *a, uint32_t insn)
{
    LOAD_ARGS
    tcg_gen_atomic_fetch_or_tl(source2, source1, source2, ctx->mem_idx, MOP);
    RESULT_AND_FREE
    return true;
}

static bool trans_AMOMINW(DisasContext *ctx, arg_AMOMINW *a, uint32_t insn)
{
    gen_amo_minmax(ctx, TCG_COND_LT, a->rd, a->rs1, a->rs2, a->rl, a->aq, MOP);
    return true;
}

static bool trans_AMOMAXW(DisasContext *ctx, arg_AMOMAXW *a, uint32_t insn)
{
    gen_amo_minmax(ctx, TCG_COND_GT, a->rd, a->rs1, a->rs2, a->rl, a->aq, MOP);
    return true;
}

static bool trans_AMOMINUW(DisasContext *ctx, arg_AMOMINUW *a, uint32_t insn)
{
    gen_amo_minmax(ctx, TCG_COND_LTU, a->rd, a->rs1, a->rs2, a->rl, a->aq, MOP);
    return true;
}

static bool trans_AMOMAXUW(DisasContext *ctx, arg_AMOMAXUW *a, uint32_t insn)
{
    gen_amo_minmax(ctx, TCG_COND_GTU, a->rd, a->rs1, a->rs2, a->rl, a->aq, MOP);
    return true;
}
#undef MOP
