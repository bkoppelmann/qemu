static bool trans_LUI(DisasContext *ctx, arg_LUI *a, uint32_t insn)
{
    if (a->rd != 0) {
        tcg_gen_movi_tl(cpu_gpr[a->rd], a->imm);
    }
    return true;
}

static bool trans_AUIPC(DisasContext *ctx, arg_AUIPC *a, uint32_t insn)
{
    if (a->rd != 0) {
        tcg_gen_movi_tl(cpu_gpr[a->rd], a->imm);
    }
    return true;
}

static bool trans_JAL(DisasContext *ctx, arg_JAL *a, uint32_t insn)
{
    target_ulong next_pc;
    /* check misaligned: */
    next_pc = ctx->pc + a->imm;
    /* FIXME: If we reuse this for RVC, don't check for misaligned */
    if ((next_pc & 0x3) != 0) {
        gen_exception_inst_addr_mis(ctx);
        return true;
    }
    if (a->rd != 0) {
        tcg_gen_movi_tl(cpu_gpr[a->rd], ctx->next_pc);
    }
    gen_goto_tb(ctx, 0, ctx->pc + a->imm); /* must use this for safety */
    ctx->bstate = BS_BRANCH;
    return true;
}
static bool trans_JALR(DisasContext *ctx, arg_JALR *a, uint32_t insn)
{
    /* no chaining with JALR */
    TCGLabel *misaligned = NULL;
    TCGv t0 = tcg_temp_new();

    gen_get_gpr(cpu_pc, a->rs1);
    tcg_gen_addi_tl(cpu_pc, cpu_pc, a->imm);
    tcg_gen_andi_tl(cpu_pc, cpu_pc, (target_ulong)-2);

    /* FIXME: If we reuse this for RVC, don't check for misaligned */
    misaligned = gen_new_label();
    tcg_gen_andi_tl(t0, cpu_pc, 0x2);
    tcg_gen_brcondi_tl(TCG_COND_NE, t0, 0x0, misaligned);

    if (a->rd != 0) {
        tcg_gen_movi_tl(cpu_gpr[a->rd], ctx->next_pc);
    }
    tcg_gen_exit_tb(0);

    if (misaligned) {
        gen_set_label(misaligned);
        gen_exception_inst_addr_mis(ctx);
    }
    ctx->bstate = BS_BRANCH;
    tcg_temp_free(t0);
    return true;
}
static bool trans_BEQ(DisasContext *ctx, arg_BEQ *a, uint32_t insn)
{
    gen_branch_cond(ctx, TCG_COND_EQ, a->rs1, a->rs2, a->imm);
    return true;
}
static bool trans_BNE(DisasContext *ctx, arg_BNE *a, uint32_t insn)
{
    gen_branch_cond(ctx, TCG_COND_NE, a->rs1, a->rs2, a->imm);
    return true;
}
static bool trans_BLT(DisasContext *ctx, arg_BLT *a, uint32_t insn)
{
    gen_branch_cond(ctx, TCG_COND_LT, a->rs1, a->rs2, a->imm);
    return true;
}
static bool trans_BGE(DisasContext *ctx, arg_BGE *a, uint32_t insn)
{
    gen_branch_cond(ctx, TCG_COND_GE, a->rs1, a->rs2, a->imm);
    return true;
}
static bool trans_BLTU(DisasContext *ctx, arg_BLTU *a, uint32_t insn)
{
    gen_branch_cond(ctx, TCG_COND_LTU, a->rs1, a->rs2, a->imm);
    return true;
}
static bool trans_BGEU(DisasContext *ctx, arg_BGEU *a, uint32_t insn)
{
    gen_branch_cond(ctx, TCG_COND_GEU, a->rs1, a->rs2, a->imm);
    return true;
}
static bool trans_LB(DisasContext *ctx, arg_LB *a, uint32_t insn)
{
    gen_load2(ctx, MO_SB, a->rd, a->rs1, a->imm);
    return true;
}
static bool trans_LH(DisasContext *ctx, arg_LH *a, uint32_t insn)
{
    gen_load2(ctx, MO_LESW, a->rd, a->rs1, a->imm);
    return true;
}
static bool trans_LW(DisasContext *ctx, arg_LW *a, uint32_t insn)
{
    gen_load2(ctx, MO_LESL, a->rd, a->rs1, a->imm);
    return true;
}
static bool trans_LBU(DisasContext *ctx, arg_LBU *a, uint32_t insn)
{
    gen_load2(ctx, MO_UB, a->rd, a->rs1, a->imm);
    return true;
}
static bool trans_LHU(DisasContext *ctx, arg_LHU *a, uint32_t insn)
{
    gen_load2(ctx, MO_LEUW, a->rd, a->rs1, a->imm);
    return true;
}
static bool trans_SB(DisasContext *ctx, arg_SB *a, uint32_t insn)
{
    gen_store2(ctx, MO_UB, a->rs1, a->rs2, a->imm);
    return true;
}
static bool trans_SH(DisasContext *ctx, arg_SH *a, uint32_t insn)
{
    gen_store2(ctx, MO_LEUW, a->rs1, a->rs2, a->imm);
    return true;
}
static bool trans_SW(DisasContext *ctx, arg_SW *a, uint32_t insn)
{
    gen_store2(ctx, MO_LEUL, a->rs1, a->rs2, a->imm);
    return true;
}

static bool trans_ADDI(DisasContext *ctx, arg_ADDI *a, uint32_t insn)
{
    LOAD_ARGSI
    tcg_gen_addi_tl(source1, source1, a->imm);
    RESULT_AND_FREEI
    return true;
}
static bool trans_SLTI(DisasContext *ctx, arg_SLTI *a, uint32_t insn)
{
    LOAD_ARGSI
    tcg_gen_setcondi_tl(TCG_COND_LT, source1, source1, a->imm);
    RESULT_AND_FREEI
    return true;
}
static bool trans_SLTIU(DisasContext *ctx, arg_SLTIU *a, uint32_t insn)
{
    LOAD_ARGSI
    tcg_gen_setcondi_tl(TCG_COND_LTU, source1, source1, a->imm);
    RESULT_AND_FREEI
    return true;
}
static bool trans_XORI(DisasContext *ctx, arg_XORI *a, uint32_t insn)
{
    LOAD_ARGSI
    tcg_gen_xori_tl(source1, source1, a->imm);
    RESULT_AND_FREEI
    return true;
}
static bool trans_ORI(DisasContext *ctx, arg_ORI *a, uint32_t insn)
{
    LOAD_ARGSI
    tcg_gen_ori_tl(source1, source1, a->imm);
    RESULT_AND_FREEI
    return true;
}
static bool trans_ANDI(DisasContext *ctx, arg_ANDI *a, uint32_t insn)
{
    LOAD_ARGSI
    tcg_gen_andi_tl(source1, source1, a->imm);
    RESULT_AND_FREEI
    return true;
}
static bool trans_SLLI(DisasContext *ctx, arg_SLLI *a, uint32_t insn)
{
    LOAD_ARGSI
    if (a->shamt > TARGET_LONG_BITS) {
        gen_exception_illegal(ctx);
        return false;
    }
    tcg_gen_shli_tl(source1, source1, a->shamt);
    RESULT_AND_FREEI
    return true;
}
static bool trans_SRLI(DisasContext *ctx, arg_SRLI *a, uint32_t insn)
{
    LOAD_ARGSI
    if (a->shamt > TARGET_LONG_BITS) {
        gen_exception_illegal(ctx);
        return false;
    }
    tcg_gen_shri_tl(source1, source1, a->shamt);
    RESULT_AND_FREEI
    return true;
}
static bool trans_SRAI(DisasContext *ctx, arg_SRAI *a, uint32_t insn)
{
    LOAD_ARGSI
    if (a->shamt > TARGET_LONG_BITS) {
        gen_exception_illegal(ctx);
        return false;
    }
    tcg_gen_sari_tl(source1, source1, a->shamt);
    RESULT_AND_FREEI
    return true;
}
static bool trans_ADD(DisasContext *ctx, arg_ADD *a, uint32_t insn)
{
    LOAD_ARGS
    tcg_gen_add_tl(source1, source1, source2);
    RESULT_AND_FREE
    return true;
}
static bool trans_SUB(DisasContext *ctx, arg_SUB *a, uint32_t insn)
{
    LOAD_ARGS
    tcg_gen_sub_tl(source1, source1, source2);
    RESULT_AND_FREE
    return true;
}
static bool trans_SLL(DisasContext *ctx, arg_SLL *a, uint32_t insn)
{
    LOAD_ARGS
    tcg_gen_andi_tl(source2, source2, TARGET_LONG_BITS - 1);
    tcg_gen_shl_tl(source1, source1, source2);
    RESULT_AND_FREE
    return true;
}
static bool trans_SLT(DisasContext *ctx, arg_SLT *a, uint32_t insn)
{
    LOAD_ARGS
    tcg_gen_setcond_tl(TCG_COND_LT, source1, source1, source2);
    RESULT_AND_FREE
    return true;
}
static bool trans_SLTU(DisasContext *ctx, arg_SLTU *a, uint32_t insn)
{
    LOAD_ARGS
    tcg_gen_setcond_tl(TCG_COND_LTU, source1, source1, source2);
    RESULT_AND_FREE
    return true;
}
static bool trans_XOR(DisasContext *ctx, arg_XOR *a, uint32_t insn)
{
    LOAD_ARGS
    tcg_gen_xor_tl(source1, source1, source2);
    RESULT_AND_FREE
    return true;
}
static bool trans_SRL(DisasContext *ctx, arg_SRL *a, uint32_t insn)
{
    LOAD_ARGS
    tcg_gen_andi_tl(source2, source2, TARGET_LONG_BITS - 1);
    tcg_gen_shr_tl(source1, source1, source2);
    RESULT_AND_FREE
    return true;
}
static bool trans_SRA(DisasContext *ctx, arg_SRA *a, uint32_t insn)
{
    LOAD_ARGS
    tcg_gen_andi_tl(source2, source2, TARGET_LONG_BITS - 1);
    tcg_gen_sar_tl(source1, source1, source2);
    RESULT_AND_FREE
    return true;
}
static bool trans_OR(DisasContext *ctx, arg_OR *a, uint32_t insn)
{
    LOAD_ARGS
    tcg_gen_or_tl(source1, source1, source2);
    RESULT_AND_FREE
    return true;
}
static bool trans_AND(DisasContext *ctx, arg_AND *a, uint32_t insn)
{
    LOAD_ARGS
    tcg_gen_and_tl(source1, source1, source2);
    RESULT_AND_FREE
    return true;
}
static bool trans_FENCE(DisasContext *ctx, arg_FENCE *a, uint32_t insn)
{
    tcg_gen_mb(TCG_MO_ALL | TCG_BAR_SC);
    return true;
}
static bool trans_FENCEI(DisasContext *ctx, arg_FENCEI *a, uint32_t insn)
{
    /* FENCE_I is a no-op in QEMU,
     * however we need to end the translation block */
    tcg_gen_movi_tl(cpu_pc, ctx->next_pc);
    tcg_gen_exit_tb(0);
    return false;
}
static bool trans_ECALL(DisasContext *ctx, arg_ECALL *a, uint32_t insn)
{
    /* always generates U-level ECALL, fixed in do_interrupt handler */
    tcg_gen_movi_tl(cpu_pc, ctx->pc);
    generate_exception(ctx, RISCV_EXCP_U_ECALL);
    tcg_gen_exit_tb(0); /* no chaining */
    ctx->bstate = BS_BRANCH;
    return false;
}
static bool trans_EBREAK(DisasContext *ctx, arg_EBREAK *a, uint32_t insn)
{
    tcg_gen_movi_tl(cpu_pc, ctx->pc);
    generate_exception(ctx, RISCV_EXCP_BREAKPOINT);
    tcg_gen_exit_tb(0); /* no chaining */
    ctx->bstate = BS_BRANCH;
    return false;
}

static bool trans_CSRRW(DisasContext *ctx, arg_CSRRW *a, uint32_t insn)
{
    TCGv source1, csr_store, dest;
    source1 = tcg_temp_new();
    csr_store = tcg_temp_new();
    dest = tcg_temp_new();

    tcg_gen_movi_tl(cpu_pc, ctx->pc);
    gen_get_gpr(source1, a->rs1);
    tcg_gen_movi_tl(csr_store, a->csr);
    gen_helper_csrrw(dest, cpu_env, source1, csr_store);
    gen_set_gpr(a->rd, dest);

    tcg_gen_movi_tl(cpu_pc, ctx->next_pc);
    tcg_gen_exit_tb(0); /* no chaining */
    ctx->bstate = BS_BRANCH;

    tcg_temp_free(source1);
    tcg_temp_free(csr_store);
    tcg_temp_free(dest);

    return false;
}
static bool trans_CSRRS(DisasContext *ctx, arg_CSRRS *a, uint32_t insn)
{
    TCGv source1, csr_store, dest, rs1_pass;
    source1 = tcg_temp_new();
    csr_store = tcg_temp_new();
    dest = tcg_temp_new();
    rs1_pass = tcg_temp_new();

    tcg_gen_movi_tl(cpu_pc, ctx->pc);
    gen_get_gpr(source1, a->rs1);
    tcg_gen_movi_tl(csr_store, a->csr);
    tcg_gen_movi_tl(rs1_pass, a->rs1);
    gen_helper_csrrs(dest, cpu_env, source1, csr_store, rs1_pass);
    gen_set_gpr(a->rd, dest);

    tcg_gen_movi_tl(cpu_pc, ctx->next_pc);
    tcg_gen_exit_tb(0); /* no chaining */
    ctx->bstate = BS_BRANCH;

    tcg_temp_free(source1);
    tcg_temp_free(csr_store);
    tcg_temp_free(rs1_pass);
    tcg_temp_free(dest);

    return false;
}
static bool trans_CSRRC(DisasContext *ctx, arg_CSRRC *a, uint32_t insn)
{
    TCGv source1, csr_store, dest, rs1_pass;
    source1 = tcg_temp_new();
    csr_store = tcg_temp_new();
    dest = tcg_temp_new();
    rs1_pass = tcg_temp_new();

    tcg_gen_movi_tl(cpu_pc, ctx->pc);
    gen_get_gpr(source1, a->rs1);
    tcg_gen_movi_tl(csr_store, a->csr);
    tcg_gen_movi_tl(rs1_pass, a->rs1);
    gen_helper_csrrc(dest, cpu_env, source1, csr_store, rs1_pass);
    gen_set_gpr(a->rd, dest);

    tcg_gen_movi_tl(cpu_pc, ctx->next_pc);
    tcg_gen_exit_tb(0); /* no chaining */
    ctx->bstate = BS_BRANCH;

    tcg_temp_free(source1);
    tcg_temp_free(csr_store);
    tcg_temp_free(rs1_pass);
    tcg_temp_free(dest);

    return false;
}
static bool trans_CSRRWI(DisasContext *ctx, arg_CSRRWI *a, uint32_t insn)
{
    TCGv csr_store, dest, imm_rs1;
    csr_store = tcg_temp_new();
    imm_rs1 = tcg_temp_new();
    dest = tcg_temp_new();

    tcg_gen_movi_tl(cpu_pc, ctx->pc);
    tcg_gen_movi_tl(imm_rs1, a->rs1);
    tcg_gen_movi_tl(csr_store, a->csr);
    gen_helper_csrrw(dest, cpu_env, imm_rs1, csr_store);
    gen_set_gpr(a->rd, dest);

    tcg_gen_movi_tl(cpu_pc, ctx->next_pc);
    tcg_gen_exit_tb(0); /* no chaining */
    ctx->bstate = BS_BRANCH;

    tcg_temp_free(csr_store);
    tcg_temp_free(imm_rs1);
    tcg_temp_free(dest);
    return false;
}
static bool trans_CSRRSI(DisasContext *ctx, arg_CSRRSI *a, uint32_t insn)
{
    TCGv csr_store, dest, imm_rs1;
    csr_store = tcg_temp_new();
    imm_rs1 = tcg_temp_new();
    dest = tcg_temp_new();

    tcg_gen_movi_tl(cpu_pc, ctx->pc);
    tcg_gen_movi_tl(imm_rs1, a->rs1);
    tcg_gen_movi_tl(csr_store, a->csr);
    gen_helper_csrrs(dest, cpu_env, imm_rs1, csr_store, imm_rs1);
    gen_set_gpr(a->rd, dest);

    tcg_gen_movi_tl(cpu_pc, ctx->next_pc);
    tcg_gen_exit_tb(0); /* no chaining */
    ctx->bstate = BS_BRANCH;

    tcg_temp_free(csr_store);
    tcg_temp_free(imm_rs1);
    tcg_temp_free(dest);
    return false;
}
static bool trans_CSRRCI(DisasContext *ctx, arg_CSRRCI *a, uint32_t insn)
{
    TCGv csr_store, dest, imm_rs1;
    csr_store = tcg_temp_new();
    imm_rs1 = tcg_temp_new();
    dest = tcg_temp_new();

    tcg_gen_movi_tl(cpu_pc, ctx->pc);
    tcg_gen_movi_tl(imm_rs1, a->rs1);
    tcg_gen_movi_tl(csr_store, a->csr);
    gen_helper_csrrc(dest, cpu_env, imm_rs1, csr_store, imm_rs1);
    gen_set_gpr(a->rd, dest);

    tcg_gen_movi_tl(cpu_pc, ctx->next_pc);
    tcg_gen_exit_tb(0); /* no chaining */
    ctx->bstate = BS_BRANCH;

    tcg_temp_free(csr_store);
    tcg_temp_free(imm_rs1);
    tcg_temp_free(dest);
    return false;
}

