bool trans_FLD(DisasContext *ctx, arg_FLD *a, uint32_t insn)
{
    TCGv t0;
    EXIT_FP_DISABLED(ctx);

    t0 = tcg_temp_new();
    gen_get_gpr(t0, a->rs1);
    tcg_gen_addi_tl(t0, t0, a->imm);

    tcg_gen_qemu_ld_i64(cpu_fpr[a->rd], t0, ctx->mem_idx, MO_TEQ);
    tcg_temp_free(t0);
    return true;
}

static bool trans_FSD(DisasContext *ctx, arg_FSD *a, uint32_t insn)
{
    TCGv t0;
    EXIT_FP_DISABLED(ctx);

    t0 = tcg_temp_new();
    gen_get_gpr(t0, a->rs1);
    tcg_gen_addi_tl(t0, t0, a->imm);
    tcg_gen_qemu_st_i64(cpu_fpr[a->rs2], t0, ctx->mem_idx, MO_TEQ);
    tcg_temp_free(t0);
    return true;
}

static bool trans_FMADDD(DisasContext *ctx, arg_FMADDD *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    gen_set_rm(ctx, a->rm);
    gen_helper_fmadd_d(cpu_fpr[a->rd], cpu_env, cpu_fpr[a->rs1],
            cpu_fpr[a->rs2], cpu_fpr[a->rs3]);
    return true;
}

static bool trans_FMSUBD(DisasContext *ctx, arg_FMSUBD *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    gen_set_rm(ctx, a->rm);
    gen_helper_fmsub_d(cpu_fpr[a->rd], cpu_env, cpu_fpr[a->rs1],
            cpu_fpr[a->rs2], cpu_fpr[a->rs3]);
    return true;
}

static bool trans_FNMSUBD(DisasContext *ctx, arg_FNMSUBD *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    gen_set_rm(ctx, a->rm);
    gen_helper_fnmsub_d(cpu_fpr[a->rd], cpu_env, cpu_fpr[a->rs1],
            cpu_fpr[a->rs2], cpu_fpr[a->rs3]);
    return true;
}

static bool trans_FNMADDD(DisasContext *ctx, arg_FNMADDD *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    gen_set_rm(ctx, a->rm);
    gen_helper_fnmadd_d(cpu_fpr[a->rd], cpu_env, cpu_fpr[a->rs1],
            cpu_fpr[a->rs2], cpu_fpr[a->rs3]);
    return true;
}

static bool trans_FADDD(DisasContext *ctx, arg_FADDD *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    gen_set_rm(ctx, a->rm);
    gen_helper_fadd_d(cpu_fpr[a->rd], cpu_env, cpu_fpr[a->rs1],
                      cpu_fpr[a->rs2]);
    return true;
}

static bool trans_FSUBD(DisasContext *ctx, arg_FSUBD *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    gen_set_rm(ctx, a->rm);
    gen_helper_fsub_d(cpu_fpr[a->rd], cpu_env, cpu_fpr[a->rs1],
                      cpu_fpr[a->rs2]);
    return true;
}

static bool trans_FMULD(DisasContext *ctx, arg_FMULD *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    gen_set_rm(ctx, a->rm);
    gen_helper_fmul_d(cpu_fpr[a->rd], cpu_env, cpu_fpr[a->rs1],
                      cpu_fpr[a->rs2]);
    return true;
}

static bool trans_FDIVD(DisasContext *ctx, arg_FDIVD *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    gen_set_rm(ctx, a->rm);
    gen_helper_fdiv_d(cpu_fpr[a->rd], cpu_env, cpu_fpr[a->rs1],
                     cpu_fpr[a->rs2]);
    return true;
}

static bool trans_FSGNJD(DisasContext *ctx, arg_FSGNJD *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    if (a->rs1 == a->rs2) { /* FMOV */
        tcg_gen_mov_i64(cpu_fpr[a->rd], cpu_fpr[a->rs1]);
    } else {
        tcg_gen_deposit_i64(cpu_fpr[a->rd], cpu_fpr[a->rs2], cpu_fpr[a->rs1],
                            0, 63);
    }
    return true;

}

static bool trans_FSGNJND(DisasContext *ctx, arg_FSGNJND *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    if (a->rs1 == a->rs2) { /* FNEG */
        tcg_gen_xori_i64(cpu_fpr[a->rd], cpu_fpr[a->rs1], INT64_MIN);
    } else {
        TCGv_i64 t0 = tcg_temp_new_i64();
        tcg_gen_not_i64(t0, cpu_fpr[a->rs2]);
        tcg_gen_deposit_i64(cpu_fpr[a->rd], t0, cpu_fpr[a->rs1],
                            0, 63);
        tcg_temp_free_i64(t0);
    }
    return true;
}

static bool trans_FSGNJXD(DisasContext *ctx, arg_FSGNJXD *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    if (a->rs1 == a->rs2) { /* FABS */
        tcg_gen_andi_i64(cpu_fpr[a->rd], cpu_fpr[a->rs1], ~INT64_MIN);
    } else {
        TCGv_i64 t0 = tcg_temp_new_i64();
        tcg_gen_andi_i64(t0, cpu_fpr[a->rs2], INT64_MIN);
        tcg_gen_xor_i64(cpu_fpr[a->rd], cpu_fpr[a->rs1], t0);
        tcg_temp_free_i64(t0);
    }
    return true;
}

bool trans_FMIND(DisasContext *ctx, arg_FMIND *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    gen_helper_fmin_d(cpu_fpr[a->rd], cpu_env, cpu_fpr[a->rs1],
                      cpu_fpr[a->rs2]);
    return true;
}

static bool trans_FMAXD(DisasContext *ctx, arg_FMAXD *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    gen_helper_fmax_d(cpu_fpr[a->rd], cpu_env, cpu_fpr[a->rs1],
                      cpu_fpr[a->rs2]);
    return true;
}

static bool trans_FCVTSD(DisasContext *ctx, arg_FCVTSD *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    gen_set_rm(ctx, a->rm);
    gen_helper_fcvt_s_d(cpu_fpr[a->rd], cpu_env, cpu_fpr[a->rs1]);
    return true;
}

static bool trans_FCVTDS(DisasContext *ctx, arg_FCVTDS *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    gen_set_rm(ctx, a->rm);
    gen_helper_fcvt_d_s(cpu_fpr[a->rd], cpu_env, cpu_fpr[a->rs1]);
    return true;
}

static bool trans_FEQD(DisasContext *ctx, arg_FEQD *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_helper_feq_d(t0, cpu_env, cpu_fpr[a->rs1], cpu_fpr[a->rs2]);
    gen_set_gpr(a->rd, t0);
    tcg_temp_free(t0);
    return true;
}

static bool trans_FLTD(DisasContext *ctx, arg_FLTD *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_helper_flt_d(t0, cpu_env, cpu_fpr[a->rs1], cpu_fpr[a->rs2]);
    gen_set_gpr(a->rd, t0);
    tcg_temp_free(t0);
    return true;
}

static bool trans_FLED(DisasContext *ctx, arg_FLED *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_helper_fle_d(t0, cpu_env, cpu_fpr[a->rs1], cpu_fpr[a->rs2]);
    gen_set_gpr(a->rd, t0);
    tcg_temp_free(t0);
    return true;
}

static bool trans_FCVTWD(DisasContext *ctx, arg_FCVTWD *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_set_rm(ctx, a->rm);
    gen_helper_fcvt_w_d(t0, cpu_env, cpu_fpr[a->rs1]);
    gen_set_gpr(a->rd, t0);
    tcg_temp_free(t0);
    return true;
}

static bool trans_FCVTWUD(DisasContext *ctx, arg_FCVTWUD *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_set_rm(ctx, a->rm);
    gen_helper_fcvt_wu_d(t0, cpu_env, cpu_fpr[a->rs1]);
    gen_set_gpr(a->rd, t0);
    tcg_temp_free(t0);
    return true;
}

static bool trans_FCVTDW(DisasContext *ctx, arg_FCVTDW *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_get_gpr(t0, a->rs1);
    gen_set_rm(ctx, a->rm);
    gen_helper_fcvt_d_w(cpu_fpr[a->rd], cpu_env, t0);
    return true;
}

static bool trans_FCVTDWU(DisasContext *ctx, arg_FCVTDWU *a, uint32_t insn)
{
    EXIT_FP_DISABLED(ctx);
    TCGv t0 = tcg_temp_new();
    gen_get_gpr(t0, a->rs1);
    gen_set_rm(ctx, a->rm);
    gen_helper_fcvt_d_wu(cpu_fpr[a->rd], cpu_env, t0);
    return true;
}
