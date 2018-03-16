bool trans_FCVTLD(DisasContext *ctx, arg_FCVTLD *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_set_rm(ctx, a->rm);
    gen_helper_fcvt_l_d(t0, cpu_env, cpu_fpr[a->rs1]);
    gen_set_gpr(a->rd, t0);
    tcg_temp_free(t0);
    return true;
#else
    return false;
#endif
}

static bool trans_FCVTLUD(DisasContext *ctx, arg_FCVTLUD *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_set_rm(ctx, a->rm);
    gen_helper_fcvt_lu_d(t0, cpu_env, cpu_fpr[a->rs1]);
    gen_set_gpr(a->rd, t0);
    tcg_temp_free(t0);
    return true;
#else
    return false;
#endif
}

static bool trans_FMVXD(DisasContext *ctx, arg_FMVXD *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    EXIT_FP_DISABLED(ctx);
    gen_set_gpr(a->rd, cpu_fpr[a->rs1]);
    return true;
#else
    return false;
#endif
}

static bool trans_FCVTDL(DisasContext *ctx, arg_FCVTDL *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_get_gpr(t0, a->rs1);
    gen_set_rm(ctx, a->rm);
    gen_helper_fcvt_d_l(cpu_fpr[a->rd], cpu_env, t0);
    tcg_temp_free(t0);
    return true;
#else
    return false;
#endif
}

static bool trans_FCVTDLU(DisasContext *ctx, arg_FCVTDLU *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_get_gpr(t0, a->rs1);
    gen_set_rm(ctx, a->rm);
    gen_helper_fcvt_d_lu(cpu_fpr[a->rd], cpu_env, t0);
    tcg_temp_free(t0);
    return true;
#else
    return false;
#endif
}

static bool trans_FMVDX(DisasContext *ctx, arg_FMVDX *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_get_gpr(t0, a->rs1);
    tcg_gen_mov_tl(cpu_fpr[a->rd], t0);
    tcg_temp_free(t0);
    return true;
#else
    return false;
#endif
}
