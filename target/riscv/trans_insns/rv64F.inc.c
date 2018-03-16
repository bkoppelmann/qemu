static bool trans_FCVTLS(DisasContext *ctx, arg_FCVTLS *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_set_rm(ctx, a->rm);
    gen_helper_fcvt_l_s(t0, cpu_env, cpu_fpr[a->rs1]);
    gen_set_gpr(a->rd, t0);
    tcg_temp_free(t0);
    return true;
#else
    return false;
#endif
}

static bool trans_FCVTLUS(DisasContext *ctx, arg_FCVTLUS *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_set_rm(ctx, a->rm);
    gen_helper_fcvt_lu_s(t0, cpu_env, cpu_fpr[a->rs1]);
    gen_set_gpr(a->rd, t0);
    tcg_temp_free(t0);
    return true;
#else
    return false;
#endif
}

static bool trans_FCVTSL(DisasContext *ctx, arg_FCVTSL *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_get_gpr(t0, a->rs1);
    gen_set_rm(ctx, a->rm);
    gen_helper_fcvt_s_l(cpu_fpr[a->rd], cpu_env, t0);
    tcg_temp_free(t0);
    return true;
#else
    return false;
#endif
}

static bool trans_FCVTSLU(DisasContext *ctx, arg_FCVTSLU *a, uint32_t insn)
{
#if defined(TARGET_RISCV64)
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_get_gpr(t0, a->rs1);
    gen_set_rm(ctx, a->rm);
    gen_helper_fcvt_s_lu(cpu_fpr[a->rd], cpu_env, t0);
    tcg_temp_free(t0);
    return true;
#else
    return false;
#endif
}
