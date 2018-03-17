static bool trans_LWU(DisasContext *ctx, arg_LWU *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    gen_load2(ctx, MO_LEUL, a->rd, a->rs1, a->imm);
    return true;
#else
    gen_exception_illegal(ctx);
    return false;
#endif

}

static bool trans_LD(DisasContext *ctx, arg_LD *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    gen_load2(ctx, MO_LEQ, a->rd, a->rs1, a->imm);
    return true;
#else
    gen_exception_illegal(ctx);
    return false;
#endif

}

static bool trans_SD(DisasContext *ctx, arg_SD *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    gen_store2(ctx, MO_LEQ, a->rs1, a->rs2, a->imm);
    return true;
#else
    gen_exception_illegal(ctx);
    return false;
#endif

}

static bool trans_ADDIW(DisasContext *ctx, arg_ADDIW *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    LOAD_ARGSI
    tcg_gen_addi_tl(source1, source1, a->imm);
    RESULT_AND_FREEI
    return true;
#else
    gen_exception_illegal(ctx);
    return false;
#endif

}

static bool trans_SLLIW(DisasContext *ctx, arg_SLLIW *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    LOAD_ARGSI
    if (a->shamt > 32) {
        gen_exception_illegal(ctx);
        return false;
    }
    tcg_gen_shli_tl(source1, source1, a->shamt);
    tcg_gen_ext32s_tl(source1, source1);
    RESULT_AND_FREEI
    return true;
#else
    gen_exception_illegal(ctx);
    return false;
#endif

}

static bool trans_SRLIW(DisasContext *ctx, arg_SRLIW *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    LOAD_ARGSI
    if (a->shamt > 32) {
        gen_exception_illegal(ctx);
        return false;
    }
    tcg_gen_extract_tl(source1, source1, a->shamt, 32 - a->shamt);
    RESULT_AND_FREEI
    return true;
#else
    gen_exception_illegal(ctx);
    return false;
#endif

}

static bool trans_SRAIW(DisasContext *ctx, arg_SRAIW *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    LOAD_ARGSI
    if (a->shamt > 32) {
        gen_exception_illegal(ctx);
        return false;
    }
    tcg_gen_sextract_tl(source1, source1, a->shamt, 32 - a->shamt);
    RESULT_AND_FREEI
    return true;
#else
    gen_exception_illegal(ctx);
    return false;
#endif

}
static bool trans_ADDW(DisasContext *ctx, arg_ADDW *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    LOAD_ARGS
    tcg_gen_add_tl(source1, source1, source2);
    tcg_gen_ext32s_tl(source1, source1);
    RESULT_AND_FREE
    return true;
#else
    gen_exception_illegal(ctx);
    return false;
#endif

}

static bool trans_SUBW(DisasContext *ctx, arg_SUBW *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    LOAD_ARGS
    tcg_gen_sub_tl(source1, source1, source2);
    tcg_gen_ext32s_tl(source1, source1);
    RESULT_AND_FREE
    return true;
#else
    gen_exception_illegal(ctx);
    return false;
#endif

}

static bool trans_SLLW(DisasContext *ctx, arg_SLLW *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    LOAD_ARGS
    tcg_gen_andi_tl(source2, source2, 0x1F);
    tcg_gen_shl_tl(source1, source1, source2);
    tcg_gen_andi_tl(source2, source2, TARGET_LONG_BITS - 1);
    tcg_gen_shl_tl(source1, source1, source2);
    tcg_gen_ext32s_tl(source1, source1);
    RESULT_AND_FREE
    return true;
#else
    gen_exception_illegal(ctx);
    return false;
#endif
}

static bool trans_SRLW(DisasContext *ctx, arg_SRLW *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    LOAD_ARGS
    tcg_gen_ext32u_tl(source1, source1);
    tcg_gen_andi_tl(source2, source2, 0x1F);
    tcg_gen_shr_tl(source1, source1, source2);
    tcg_gen_andi_tl(source2, source2, TARGET_LONG_BITS - 1);
    tcg_gen_shr_tl(source1, source1, source2);
    RESULT_AND_FREE
    return true;
#else
    gen_exception_illegal(ctx);
    return false;
#endif
}

static bool trans_SRAW(DisasContext *ctx, arg_SRAW *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    LOAD_ARGS
    /* first, trick to get it to act like working on 32 bits (get rid of
    upper 32, sign extend to fill space) */
    tcg_gen_ext32s_tl(source1, source1);
    tcg_gen_andi_tl(source2, source2, 0x1F);
    tcg_gen_sar_tl(source1, source1, source2);
    tcg_gen_andi_tl(source2, source2, TARGET_LONG_BITS - 1);
    tcg_gen_sar_tl(source1, source1, source2);
    RESULT_AND_FREE
    return true;
#else
    gen_exception_illegal(ctx);
    return false;
#endif
}

