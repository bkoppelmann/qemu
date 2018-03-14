#define MOP (MO_ALIGN | MO_TESL)
bool trans_LRW(DisasContext *ctx, arg_LRW *a, uint32_t insn)
{
    TCGv src1 = tcg_temp_new();

    gen_get_gpr(src1, a->rs1);
    if (a->rl) {
        tcg_gen_mb(TCG_MO_ALL | TCG_BAR_STRL);
    }
    tcg_gen_qemu_ld_tl(load_val, src1, ctx->mem_idx, MOP);
    if (a->aq) {
        tcg_gen_mb(TCG_MO_ALL | TCG_BAR_LDAQ);
    }
    tcg_gen_mov_tl(load_res, src1);
    gen_set_gpr(a->rd, load_val);

    tcg_temp_free(src1);
    return true;
}

static bool trans_SCW(DisasContext *ctx, arg_SCW *a, uint32_t insn)
{
    TCGLabel *l1, *l2;
    TCGv src1 = tcg_temp_new();
    TCGv src2 = tcg_temp_new();
    TCGv dat  = tcg_temp_new();

    l1 = gen_new_label();
    l2 = gen_new_label();
    gen_get_gpr(src1, a->rs1);
    tcg_gen_brcond_tl(TCG_COND_NE, load_res, src1, l1);

    gen_get_gpr(src2, a->rs2);
    /* Note that the TCG atomic primitives are SC,
       so we can ignore AQ/RL along this path.  */
    tcg_gen_atomic_cmpxchg_tl(src1, load_res, load_val, src2,
                              ctx->mem_idx, MOP);
    tcg_gen_setcond_tl(TCG_COND_NE, dat, src1, load_val);
    gen_set_gpr(a->rd, dat);
    tcg_gen_br(l2);

    gen_set_label(l1);
    /* Address comparion failure.  However, we still need to
       provide the memory barrier implied by AQ/RL.  */
    tcg_gen_mb(TCG_MO_ALL + a->aq * TCG_BAR_LDAQ + a->rl * TCG_BAR_STRL);
    tcg_gen_movi_tl(dat, 1);
    gen_set_gpr(a->rd, dat);

    gen_set_label(l2);
    tcg_temp_free(src1);
    tcg_temp_free(src2);
    tcg_temp_free(dat);
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
