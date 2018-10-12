/*
 * RISC-V translation routines for the RVC Compressed Instruction Set.
 *
 * Copyright (c) 2016-2017 Sagar Karandikar, sagark@eecs.berkeley.edu
 * Copyright (c) 2018 Peer Adelt, peer.adelt@hni.uni-paderborn.de
 *                    Bastian Koppelmann, kbastian@mail.uni-paderborn.de
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2 or later, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

static bool trans_c_addi4spn(DisasContext *ctx, arg_c_addi4spn *a,
        uint16_t insn)
{
    if (a->nzuimm == 0) {
        /* Reserved in ISA */
        return false;
    }
    arg_addi arg = { .rd = a->rd, .rs1 = 2, .imm = a->nzuimm };
    return trans_addi(ctx, &arg, insn);
}

static bool trans_c_fld(DisasContext *ctx, arg_c_fld *a, uint16_t insn)
{
    arg_fld arg = { .rd = a->rd, .rs1 = a->rs1, .imm = a->uimm };
    return trans_fld(ctx, &arg, insn);
}

static bool trans_c_lw(DisasContext *ctx, arg_c_lw *a, uint16_t insn)
{
    arg_lw arg = { .rd = a->rd, .rs1 = a->rs1, .imm = a->uimm };
    return trans_lw(ctx, &arg, insn);
}

static bool trans_c_flw_ld(DisasContext *ctx, arg_c_flw_ld *a, uint16_t insn)
{
#ifdef TARGET_RISCV32
    /* C.FLW ( RV32FC-only ) */
    arg_c_lw *tmp = g_new0(arg_c_lw, 1);
    extract_cl_w(tmp, insn);
    arg_flw arg = { .rd = tmp->rd, .rs1 = tmp->rs1, .imm = tmp->uimm };
    return trans_flw(ctx, &arg, insn);
#else
    /* C.LD ( RV64C/RV128C-only ) */
    arg_c_fld *tmp = g_new0(arg_c_fld, 1);
    extract_cl_d(tmp, insn);
    arg_ld arg = { .rd = tmp->rd, .rs1 = tmp->rs1, .imm = tmp->uimm };
    return trans_ld(ctx, &arg, insn);
#endif
}

static bool trans_c_fsd(DisasContext *ctx, arg_c_fsd *a, uint16_t insn)
{
    arg_fsd arg = { .rs1 = a->rs1, .rs2 = a->rs2, .imm = a->uimm };
    return trans_fsd(ctx, &arg, insn);
}

static bool trans_c_sw(DisasContext *ctx, arg_c_sw *a, uint16_t insn)
{
    arg_sw arg = { .rs1 = a->rs1, .rs2 = a->rs2, .imm = a->uimm };
    return trans_sw(ctx, &arg, insn);
}

static bool trans_c_fsw_sd(DisasContext *ctx, arg_c_fsw_sd *a, uint16_t insn)
{
#ifdef TARGET_RISCV32
    /* C.FSW ( RV32FC-only ) */
    arg_c_sw *tmp = g_new0(arg_c_sw, 1);
    extract_cs_w(tmp, insn);
    arg_fsw arg = { .rs1 = tmp->rs1, .rs2 = tmp->rs2, .imm = tmp->uimm };
    return trans_fsw(ctx, &arg, insn);
#else
    /* C.SD ( RV64C/RV128C-only ) */
    arg_c_fsd *tmp = g_new0(arg_c_fsd, 1);
    extract_cs_d(tmp, insn);
    arg_sd arg = { .rs1 = tmp->rs1, .rs2 = tmp->rs2, .imm = tmp->uimm };
    return trans_sd(ctx, &arg, insn);
#endif
}

static bool trans_c_addi(DisasContext *ctx, arg_c_addi *a, uint16_t insn)
{
    if (a->imm == 0) {
        return true;
    }
    arg_addi arg = { .rd = a->rd, .rs1 = a->rd, .imm = a->imm };
    return trans_addi(ctx, &arg, insn);
}

static bool trans_c_jal_addiw(DisasContext *ctx, arg_c_jal_addiw *a,
        uint16_t insn)
{
#ifdef TARGET_RISCV32
    /* C.JAL */
    arg_c_j *tmp = g_new0(arg_c_j, 1);
    extract_cj(tmp, insn);
    arg_jal arg = { .rd = 1, .imm = a->imm };
    return trans_jal(ctx, &arg, insn);
#else
    /* C.ADDIW */
    arg_addiw arg = { .rd = a->rd, .rs1 = a->rd, .imm = a->imm };
    return trans_addiw(ctx, &arg, insn);
#endif
}

static bool trans_c_li(DisasContext *ctx, arg_c_li *a, uint16_t insn)
{
    if (a->rd == 0) {
        return true;
    }
    arg_addi arg = { .rd = a->rd, .rs1 = 0, .imm = a->imm };
    return trans_addi(ctx, &arg, insn);
}

static bool trans_c_addi16sp_lui(DisasContext *ctx, arg_c_addi16sp_lui *a,
        uint16_t insn)
{
    if (a->rd == 2) {
        /* C.ADDI16SP */
        arg_addi arg = { .rd = 2, .rs1 = 2, .imm = a->imm_addi16sp };
        return trans_addi(ctx, &arg, insn);
    } else if (a->imm_lui != 0) {
        if (a->rd == 0) {
            return true;
        }
        /* C.LUI */
        arg_lui arg = { .rd = a->rd, .imm = a->imm_lui };
        return trans_lui(ctx, &arg, insn);
    }
    return false;
}

static bool trans_c_srli(DisasContext *ctx, arg_c_srli *a, uint16_t insn)
{
    if (a->shamt == 0) {
        /* Reserved in ISA */
        return false;
    }
#ifdef TARGET_RISCV32
    /* Ensure, that shamt[5] is zero for RV32 */
    if (a->shamt >= 32) {
        return false;
    }
#endif

    arg_srli arg = { .rd = a->rd, .rs1 = a->rd, .shamt = a->shamt };
    return trans_srli(ctx, &arg, insn);
}

static bool trans_c_srai(DisasContext *ctx, arg_c_srai *a, uint16_t insn)
{
    if (a->shamt == 0) {
        /* Reserved in ISA */
        return false;
    }
#ifdef TARGET_RISCV32
    /* Ensure, that shamt[5] is zero for RV32 */
    if (a->shamt >= 32) {
        return false;
    }
#endif

    arg_srai arg = { .rd = a->rd, .rs1 = a->rd, .shamt = a->shamt };
    return trans_srai(ctx, &arg, insn);
}

static bool trans_c_andi(DisasContext *ctx, arg_c_andi *a, uint16_t insn)
{
    arg_andi arg = { .rd = a->rd, .rs1 = a->rd, .imm = a->imm };
    return trans_andi(ctx, &arg, insn);
}

static bool trans_c_sub(DisasContext *ctx, arg_c_sub *a, uint16_t insn)
{
    arg_sub arg = { .rd = a->rd, .rs1 = a->rd, .rs2 = a->rs2 };
    return trans_sub(ctx, &arg, insn);
}

static bool trans_c_xor(DisasContext *ctx, arg_c_xor *a, uint16_t insn)
{
    arg_xor arg = { .rd = a->rd, .rs1 = a->rd, .rs2 = a->rs2 };
    return trans_xor(ctx, &arg, insn);
}

static bool trans_c_or(DisasContext *ctx, arg_c_or *a, uint16_t insn)
{
    arg_or arg = { .rd = a->rd, .rs1 = a->rd, .rs2 = a->rs2 };
    return trans_or(ctx, &arg, insn);
}

static bool trans_c_and(DisasContext *ctx, arg_c_and *a, uint16_t insn)
{
    arg_and arg = { .rd = a->rd, .rs1 = a->rd, .rs2 = a->rs2 };
    return trans_and(ctx, &arg, insn);
}

static bool trans_c_subw(DisasContext *ctx, arg_c_subw *a, uint16_t insn)
{
    arg_subw arg = { .rd = a->rd, .rs1 = a->rd, .rs2 = a->rs2 };
    return trans_subw(ctx, &arg, insn);
}

static bool trans_c_addw(DisasContext *ctx, arg_c_addw *a, uint16_t insn)
{
    arg_addw arg = { .rd = a->rd, .rs1 = a->rd, .rs2 = a->rs2 };
    return trans_addw(ctx, &arg, insn);
}

static bool trans_c_j(DisasContext *ctx, arg_c_j *a, uint16_t insn)
{
    arg_jal arg = { .rd = 0, .imm = a->imm };
    return trans_jal(ctx, &arg, insn);
}

static bool trans_c_beqz(DisasContext *ctx, arg_c_beqz *a, uint16_t insn)
{
    arg_beq arg = { .rs1 = a->rs1, .rs2 = 0, .imm = a->imm };
    return trans_beq(ctx, &arg, insn);
}

static bool trans_c_bnez(DisasContext *ctx, arg_c_bnez *a, uint16_t insn)
{
    arg_bne arg = { .rs1 = a->rs1, .rs2 = 0, .imm = a->imm };
    return trans_bne(ctx, &arg, insn);
}

static bool trans_c_slli(DisasContext *ctx, arg_c_slli *a, uint16_t insn)
{
    if (a->shamt == 0) {
        /* Reserved in ISA */
        return false;
    }

#ifdef TARGET_RISCV32
    /* Ensure, that shamt[5] is zero for RV32 */
    if (a->shamt >= 32) {
        return false;
    }
#endif

    arg_slli arg = { .rd = a->rd, .rs1 = a->rd, .shamt = a->shamt };
    return trans_slli(ctx, &arg, insn);
}

static bool trans_c_fldsp(DisasContext *ctx, arg_c_fldsp *a, uint16_t insn)
{
    arg_fld arg = { .rd = a->rd, .rs1 = 2, .imm = a->uimm };
    return trans_fld(ctx, &arg, insn);
}

static bool trans_c_lwsp(DisasContext *ctx, arg_c_lwsp *a, uint16_t insn)
{
    arg_lw arg = { .rd = a->rd, .rs1 = 2, .imm = a->uimm };
    return trans_lw(ctx, &arg, insn);
}

static bool trans_c_flwsp_ldsp(DisasContext *ctx, arg_c_flwsp_ldsp *a,
        uint16_t insn)
{
#ifdef TARGET_RISCV32
    /* C.FLWSP */
    arg_flw arg_flw = { .rd = a->rd, .rs1 = 2, .imm = a->uimm_flwsp };
    return trans_flw(ctx, &arg_flw, insn);
#else
    /* C.LDSP */
    arg_ld arg_ld = { .rd = a->rd, .rs1 = 2, .imm = a->uimm_ldsp };
    return trans_ld(ctx, &arg_ld, insn);
#endif
    return false;
}

static bool trans_c_jr_mv(DisasContext *ctx, arg_c_jr_mv *a, uint16_t insn)
{
    if (a->rd != 0 && a->rs2 == 0) {
        /* C.JR */
        arg_jalr arg = { .rd = 0, .rs1 = a->rd, .imm = 0 };
        return trans_jalr(ctx, &arg, insn);
    } else if (a->rd != 0 && a->rs2 != 0) {
        /* C.MV */
        arg_add arg = { .rd = a->rd, .rs1 = 0, .rs2 = a->rs2 };
        return trans_add(ctx, &arg, insn);
    }
    return false;
}

static bool trans_c_ebreak_jalr_add(DisasContext *ctx, arg_c_ebreak_jalr_add *a,
        uint16_t insn)
{
    if (a->rd == 0 && a->rs2 == 0) {
        /* C.EBREAK */
        arg_ebreak arg = { };
        return trans_ebreak(ctx, &arg, insn);
    } else if (a->rd != 0) {
        if (a->rs2 == 0) {
            /* C.JALR */
            arg_jalr arg = { .rd = 1, .rs1 = a->rd, .imm = 0 };
            return trans_jalr(ctx, &arg, insn);
        } else {
            /* C.ADD */
            arg_add arg = { .rd = a->rd, .rs1 = a->rd, .rs2 = a->rs2 };
            return trans_add(ctx, &arg, insn);
        }
    }
    return false;
}

static bool trans_c_fsdsp(DisasContext *ctx, arg_c_fsdsp *a, uint16_t insn)
{
    arg_fsd arg = { .rs1 = 2, .rs2 = a->rs2, .imm = a->uimm };
    return trans_fsd(ctx, &arg, insn);
}

static bool trans_c_swsp(DisasContext *ctx, arg_c_swsp *a, uint16_t insn)
{
    arg_sw arg = { .rs1 = 2, .rs2 = a->rs2, .imm = a->uimm };
    return trans_sw(ctx, &arg, insn);
}

static bool trans_c_fswsp_sdsp(DisasContext *ctx, arg_c_fswsp_sdsp *a,
        uint16_t insn)
{
#ifdef TARGET_RISCV32
    /* C.FSWSP */
    arg_fsw a_fsw = { .rs1 = a->rs2, .rs2 = 2, .imm = a->uimm_fswsp };
    return trans_fsw(ctx, &a_fsw, insn);
#endif
    /* C.SDSP */
    arg_sd a_sd = { .rs1 = 2, .rs2 = a->rs2, .imm = a->uimm_sdsp };
    return trans_sd(ctx, &a_sd, insn);
}
