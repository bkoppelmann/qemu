#define MOP (MO_ALIGN | MO_TEQ)
static bool trans_LRD(DisasContext *ctx, arg_LRD *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    gen_lrwd(ctx, a->rd, a->rs1, a->rl, a->aq, MOP);
    return true;
#else
    gen_exception_illegal(ctx);
    return false;
#endif
}

static bool trans_SCD(DisasContext *ctx, arg_SCD *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    gen_scwd(ctx, a->rd, a->rs1, a->rs2, a->rl, a->aq, MOP);
    return true;
#else
    gen_exception_illegal(ctx);
    return false;
#endif
}

static bool trans_AMOSWAPD(DisasContext *ctx, arg_AMOSWAPD *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    LOAD_ARGS
    tcg_gen_atomic_xchg_tl(source2, source1, source2, ctx->mem_idx, MOP);
    gen_set_gpr(a->rd, source2); 
    tcg_temp_free(source1); 
    tcg_temp_free(source2);
    return true;
#else
    gen_exception_illegal(ctx);
    return false;
#endif
}

static bool trans_AMOADDD(DisasContext *ctx, arg_AMOADDD *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    LOAD_ARGS
    tcg_gen_atomic_fetch_add_tl(source2, source1, source2, ctx->mem_idx, MOP);
    gen_set_gpr(a->rd, source2); 
    tcg_temp_free(source1); 
    tcg_temp_free(source2);
    return true;
#else
    gen_exception_illegal(ctx);
    return false;
#endif
}

static bool trans_AMOXORD(DisasContext *ctx, arg_AMOXORD *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    LOAD_ARGS
    tcg_gen_atomic_fetch_xor_tl(source2, source1, source2, ctx->mem_idx, MOP);
    gen_set_gpr(a->rd, source2); 
    tcg_temp_free(source1); 
    tcg_temp_free(source2);
    return true;
#else
    gen_exception_illegal(ctx);
    return false;
#endif
}

static bool trans_AMOANDD(DisasContext *ctx, arg_AMOANDD *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    LOAD_ARGS
    tcg_gen_atomic_fetch_and_tl(source2, source1, source2, ctx->mem_idx, MOP);
    gen_set_gpr(a->rd, source2); 
    tcg_temp_free(source1); 
    tcg_temp_free(source2);
    return true;
#else
    gen_exception_illegal(ctx);
    return false;
#endif
}

static bool trans_AMOORD(DisasContext *ctx, arg_AMOORD *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    LOAD_ARGS
    tcg_gen_atomic_fetch_or_tl(source2, source1, source2, ctx->mem_idx, MOP);
    gen_set_gpr(a->rd, source2); 
    tcg_temp_free(source1); 
    tcg_temp_free(source2);
    return true;
#else
    gen_exception_illegal(ctx);
    return false;
#endif
}

static bool trans_AMOMIND(DisasContext *ctx, arg_AMOMIND *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    gen_amo_minmax(ctx, TCG_COND_LT, a->rd, a->rs1, a->rs2, a->rl, a->aq, MOP);
    return true;
#else
    gen_exception_illegal(ctx);
    return false;
#endif
}

static bool trans_AMOMAXD(DisasContext *ctx, arg_AMOMAXD *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    gen_amo_minmax(ctx, TCG_COND_GT, a->rd, a->rs1, a->rs2, a->rl, a->aq, MOP);
    return true;
#else
    gen_exception_illegal(ctx);
    return false;
#endif
}

static bool trans_AMOMINUD(DisasContext *ctx, arg_AMOMINUD *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    gen_amo_minmax(ctx, TCG_COND_LTU, a->rd, a->rs1, a->rs2, a->rl, a->aq, MOP);
    return true;
#else
    gen_exception_illegal(ctx);
    return false;
#endif
}

bool trans_AMOMAXUD(DisasContext *ctx, arg_AMOMAXUD *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    gen_amo_minmax(ctx, TCG_COND_GTU, a->rd, a->rs1, a->rs2, a->rl, a->aq, MOP);
    return true;
#else
    gen_exception_illegal(ctx);
    return false;
#endif
}
#undef MOP
