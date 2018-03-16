#define EXIT_FP_DISABLED(ctx) do { \
    if (!(ctx->flags & TB_FLAGS_FP_ENABLE)) { \
        gen_exception_illegal(ctx); \
        return false; \
    } \
} while(0)

bool trans_FLW(DisasContext *ctx, arg_FLW *a, uint32_t insn)
{
    TCGv t0;
    EXIT_FP_DISABLED(ctx);

    t0 = tcg_temp_new();
    gen_get_gpr(t0, a->rs1);
    tcg_gen_addi_tl(t0, t0, a->imm);

    tcg_gen_qemu_ld_i64(cpu_fpr[a->rd], t0, ctx->mem_idx, MO_TEUL);
    /* RISC-V requires NaN-boxing of narrower width floating point values */
    tcg_gen_ori_i64(cpu_fpr[a->rd], cpu_fpr[a->rd], 0xffffffff00000000ULL);
    tcg_temp_free(t0);
    return true;
}

static bool trans_FSW(DisasContext *ctx, arg_FSW *a, uint32_t insn)
{
    TCGv t0;
    EXIT_FP_DISABLED(ctx);

    t0 = tcg_temp_new();
    gen_get_gpr(t0, a->rs1);
    tcg_gen_addi_tl(t0, t0, a->imm);
    tcg_gen_qemu_st_i64(cpu_fpr[a->rs2], t0, ctx->mem_idx, MO_TEUL);
    tcg_temp_free(t0);
    return true;
}

static bool trans_FMADDS(DisasContext *ctx, arg_FMADDS *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    gen_set_rm(ctx, a->rm);
    gen_helper_fmadd_s(cpu_fpr[a->rd], cpu_env, cpu_fpr[a->rs1],
            cpu_fpr[a->rs2], cpu_fpr[a->rs3]);
    return true;
}

static bool trans_FMSUBS(DisasContext *ctx, arg_FMSUBS *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    gen_set_rm(ctx, a->rm);
    gen_helper_fmsub_s(cpu_fpr[a->rd], cpu_env, cpu_fpr[a->rs1],
            cpu_fpr[a->rs2], cpu_fpr[a->rs3]);
    return true;
}

static bool trans_FNMSUBS(DisasContext *ctx, arg_FNMSUBS *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    gen_set_rm(ctx, a->rm);
    gen_helper_fnmsub_s(cpu_fpr[a->rd], cpu_env, cpu_fpr[a->rs1],
            cpu_fpr[a->rs2], cpu_fpr[a->rs3]);

    return true;
}

static bool trans_FNMADDS(DisasContext *ctx, arg_FNMADDS *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    gen_set_rm(ctx, a->rm);
    gen_helper_fnmadd_s(cpu_fpr[a->rd], cpu_env, cpu_fpr[a->rs1],
            cpu_fpr[a->rs2], cpu_fpr[a->rs3]);

    return true;
}

static bool trans_FADDS(DisasContext *ctx, arg_FADDS *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    gen_set_rm(ctx, a->rm);
    gen_helper_fadd_s(cpu_fpr[a->rd], cpu_env, cpu_fpr[a->rs1],
            cpu_fpr[a->rs2]);
    return true;
}

static bool trans_FSUBS(DisasContext *ctx, arg_FSUBS *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    gen_set_rm(ctx, a->rm);
    gen_helper_fsub_s(cpu_fpr[a->rd], cpu_env, cpu_fpr[a->rs1],
            cpu_fpr[a->rs2]);
    return true;
}

static bool trans_FMULS(DisasContext *ctx, arg_FMULS *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    gen_set_rm(ctx, a->rm);
    gen_helper_fmul_s(cpu_fpr[a->rd], cpu_env, cpu_fpr[a->rs1],
            cpu_fpr[a->rs2]);
    return true;
}

static bool trans_FDIVS(DisasContext *ctx, arg_FDIVS *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    gen_set_rm(ctx, a->rm);
    gen_helper_fdiv_s(cpu_fpr[a->rd], cpu_env, cpu_fpr[a->rs1],
            cpu_fpr[a->rs2]);
    return true;
}

static bool trans_FSQRTS(DisasContext *ctx, arg_FSQRTS *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    gen_set_rm(ctx, a->rm);
    gen_helper_fsqrt_s(cpu_fpr[a->rd], cpu_env, cpu_fpr[a->rs1]);
    return true;
}

static bool trans_FSGNJS(DisasContext *ctx, arg_FSGNJS *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    if (a->rs1 == a->rs2) { /* FMOV */
        tcg_gen_mov_i64(cpu_fpr[a->rd], cpu_fpr[a->rs1]);
    } else {
        tcg_gen_deposit_i64(cpu_fpr[a->rd], cpu_fpr[a->rs2], cpu_fpr[a->rs1],
                            0, 31);
    }
    return true;
}

static bool trans_FSGNJNS(DisasContext *ctx, arg_FSGNJNS *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
   if (a->rs1 == a->rs2) { /* FNEG */
        tcg_gen_xori_i64(cpu_fpr[a->rd], cpu_fpr[a->rs1], INT32_MIN);
    } else {
        TCGv_i64 t0 = tcg_temp_new_i64();
        tcg_gen_not_i64(t0, cpu_fpr[a->rs2]);
        tcg_gen_deposit_i64(cpu_fpr[a->rd], t0, cpu_fpr[a->rs1],
                            0, 31);
        tcg_temp_free_i64(t0);
    }
    return true;
}

static bool trans_FSGNJXS(DisasContext *ctx, arg_FSGNJXS *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    if (a->rs1 == a->rs2) { /* FABS */
        tcg_gen_andi_i64(cpu_fpr[a->rd], cpu_fpr[a->rs1], ~INT32_MIN);
    } else {
        TCGv_i64 t0 = tcg_temp_new_i64();
        tcg_gen_andi_i64(t0, cpu_fpr[a->rs2], INT32_MIN);
        tcg_gen_xor_i64(cpu_fpr[a->rd], cpu_fpr[a->rs1], t0);
        tcg_temp_free_i64(t0);
    }
    return true;
}

static bool trans_FMINS(DisasContext *ctx, arg_FMINS *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    gen_helper_fmin_s(cpu_fpr[a->rd], cpu_env, cpu_fpr[a->rs1],
                      cpu_fpr[a->rs2]);
    return true;
}

static bool trans_FMAXS(DisasContext *ctx, arg_FMAXS *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    gen_helper_fmax_s(cpu_fpr[a->rd], cpu_env, cpu_fpr[a->rs1],
                      cpu_fpr[a->rs2]);
    return true;
}

static bool trans_FCVTWS(DisasContext *ctx, arg_FCVTWS *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_set_rm(ctx, a->rm);
    gen_helper_fcvt_w_s(t0, cpu_env, cpu_fpr[a->rs1]);
    gen_set_gpr(a->rd, t0);
    tcg_temp_free(t0);
    return true;
}

static bool trans_FCVTWUS(DisasContext *ctx, arg_FCVTWUS *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_set_rm(ctx, a->rm);
    gen_helper_fcvt_wu_s(t0, cpu_env, cpu_fpr[a->rs1]);
    gen_set_gpr(a->rd, t0);
    tcg_temp_free(t0);
    return true;
}

static bool trans_FMVXW(DisasContext *ctx, arg_FMVXW *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
#if defined(TARGET_RISCV64)
    tcg_gen_ext32s_tl(t0, cpu_fpr[a->rs1]);
#else
    tcg_gen_extrl_i64_i32(t0, cpu_fpr[a->rs1]);
#endif
    gen_set_gpr(a->rd, t0);
    tcg_temp_free(t0);
    return true;
}

static bool trans_FEQS(DisasContext *ctx, arg_FEQS *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_helper_feq_s(t0, cpu_env, cpu_fpr[a->rs1], cpu_fpr[a->rs2]);
    gen_set_gpr(a->rd, t0);
    tcg_temp_free(t0);
    return true;
}

static bool trans_FLTS(DisasContext *ctx, arg_FLTS *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_helper_flt_s(t0, cpu_env, cpu_fpr[a->rs1], cpu_fpr[a->rs2]);
    gen_set_gpr(a->rd, t0);
    tcg_temp_free(t0);
    return true;
}

static bool trans_FLES(DisasContext *ctx, arg_FLES *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_helper_fle_s(t0, cpu_env, cpu_fpr[a->rs1], cpu_fpr[a->rs2]);
    gen_set_gpr(a->rd, t0);
    tcg_temp_free(t0);

    return true;
}

static bool trans_FCLASSS(DisasContext *ctx, arg_FCLASSS *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_helper_fclass_s(t0, cpu_fpr[a->rs1]);
    gen_set_gpr(a->rd, t0);
    tcg_temp_free(t0);
    return true;
}

static bool trans_FCVTSW(DisasContext *ctx, arg_FCVTSW *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_get_gpr(t0, a->rs1);
    gen_set_rm(ctx, a->rm);
    gen_helper_fcvt_s_w(cpu_fpr[a->rd], cpu_env, t0);
    tcg_temp_free(t0);
    return true;
}

static bool trans_FCVTSWU(DisasContext *ctx, arg_FCVTSWU *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_get_gpr(t0, a->rs1);
    gen_set_rm(ctx, a->rm);
    gen_helper_fcvt_s_wu(cpu_fpr[a->rd], cpu_env, t0);
    tcg_temp_free(t0);
    return true;
}

static bool trans_FMVWX(DisasContext *ctx, arg_FMVWX *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_get_gpr(t0, a->rs1);
#if defined(TARGET_RISCV64)
    tcg_gen_mov_i64(cpu_fpr[a->rd], t0);
#else
    tcg_gen_extu_i32_i64(cpu_fpr[a->rd], t0);
#endif
    tcg_temp_free(t0);
    return true;
}
