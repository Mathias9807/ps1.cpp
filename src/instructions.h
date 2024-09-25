#pragma once

#include "cpu.h"
#include <cstdint>


const char* mnemonics[] = {
	"special", "b?z", "j", "jal", "beq", "bne", "blez", "bgtz",
	"addi", "addiu", "slti", "sltiu", "andi", "ori", "xori", "lui",
	"cop0", "cop1", "cop2", "cop3", "?", "?", "?", "?",
	"?", "?", "?", "?", "?", "?", "?", "?",
	"lb", "lh", "lwl", "lw", "lbu", "lhu", "lwr", "?",
	"sb", "sh", "swl", "sw", "?", "?", "swr", "?",
	"lwc0", "lwc1", "lwc2", "lwc3", "?", "?", "?", "?",
	"swc0", "swc1", "swc2", "swc3", "?", "?", "?", "?",
};

const char* secondary_mnems[] = {
	"sll", "?", "srl", "sra", "sllv", "?", "srlv", "srav",
	"jr", "jalr", "?", "?", "syscall", "break", "?", "?",
	"mfhi", "mthi", "mflo", "mtlo", "?", "?", "?", "?",
	"mult", "multu", "div", "divu", "?", "?", "?", "?",
	"add", "addu", "sub", "subu", "and", "or", "xor", "nor",
	"?", "?", "slt", "sltu", "?", "?", "?", "?",
	"?", "?", "?", "?", "?", "?", "?", "?",
	"?", "?", "?", "?", "?", "?", "?", "?",
};

const char* tertiary_mnems[] = {
	"bltz", "bgez", "bltzal", "bgezal"
};

// Math instructions
void add(reg rd, reg rs, reg rt) {
	registers[rd] = registers[rs] + registers[rt];
	incr_pc(4);
}

void addi(reg rt, reg rs, reg imm) {
	registers[rt] = registers[rs] + imm;
	incr_pc(4);
}

void addiu(reg rt, reg rs, reg imm) {
	registers[rt] = registers[rs] + imm;
	incr_pc(4);
}

void addu(reg rd, reg rs, reg rt) {
	registers[rd] = registers[rs] + registers[rt];
	incr_pc(4);
}

// resource: https://chortle.ccsu.edu/assemblytutorial/Chapter-13/ass13_11.html
void sub(reg rd, reg rs, reg rt) {
	registers[rd] = registers[rs] - registers[rt];
	incr_pc(4);
}

void subu(reg rd, reg rs, reg rt) {
	registers[rd] = registers[rs] - registers[rt];
	incr_pc(4);
}

void mult(reg rs, reg rt) {
	lo = (int32_t) registers[rs] * (int32_t) registers[rt];
	incr_pc(4);
}

void multu(reg rs, reg rt) {
	lo = registers[rs] * registers[rt];
	incr_pc(4);
}

void div(reg rs, reg rt) {
	lo = (int32_t) registers[rs] / (int32_t) registers[rt];
	hi = (int32_t) registers[rs] % (int32_t) registers[rt];
	incr_pc(4);
}

void divu(reg rs, reg rt) {
	lo = registers[rs] / registers[rt];
	hi = registers[rs] % registers[rt];
	incr_pc(4);
}

// Bitwise operations
void andd(reg rd, reg rs, reg rt) {
	registers[rd] = registers[rs] & registers[rt];
	incr_pc(4);
}

void andi(reg rt, reg rs, uint16_t imm) {
	registers[rt] = registers[rs] & imm;
	incr_pc(4);
}

void orr(reg rd, reg rs, reg rt) {
	registers[rd] = registers[rs] | registers[rt];
	incr_pc(4);
}

void ori(reg rt, reg rs, uint16_t imm) {
	registers[rt] = registers[rs] | imm;
	incr_pc(4);
}

void xorr(reg rd, reg rs, reg rt) {
	registers[rd] = registers[rs] ^ registers[rt];
	incr_pc(4);
}

void xori(reg rt, reg rs, uint16_t imm) {
	registers[rt] = registers[rs] ^ imm;
	incr_pc(4);
}

// Shifts
void sll(reg rd, reg rt, uint16_t shamt) {
	rd = registers[rt] << shamt;
	incr_pc(4);
}

void sllv(reg rd, reg rt, reg rs) {
	rd = registers[rt] << registers[rs];
	incr_pc(4);
}

// ok fuck. signed right shifts in c are UB, not necessarily arithmetic
void sra(reg rd, reg rt, uint16_t shamt) {
	rd = (int32_t) registers[rt] >> (int32_t) shamt;
	incr_pc(4);
}

void srl(reg rd, reg rt, uint16_t shamt) {
	rd = registers[rt] >> shamt;
	incr_pc(4);
}

void srlv(reg rd, reg rt, reg rs) {
	rd = registers[rt] >> registers[rs];
	incr_pc(4);
}

// Jumps
void j(uint32_t address) {
	pc = npc;
	npc = (pc & 0xF0000000) | (address << 2);
}

void jal(uint32_t address) {
	registers[31] = pc + 8;
	j(address);
}

void jr(reg address) {
	pc = npc;
	npc = registers[address];
}

// Branches
void beq(reg rs, reg rt, int16_t offs) {
	if (registers[rs] == registers[rt]) incr_pc(offs << 2);
	else incr_pc(4);
}

void bne(reg rs, reg rt, int16_t offs) {
	if (registers[rs] != registers[rt]) incr_pc(offs << 2);
	else incr_pc(4);
}

void bgez(reg rs, int16_t offs) {
	if (registers[rs] >= 0) incr_pc(offs << 2);
	else incr_pc(4);
}

void bgtz(reg rs, int16_t offs) {
	if (registers[rs] > 0) incr_pc(offs << 2);
	else incr_pc(4);
}

void bltz(reg rs, int16_t offs) {
	if (registers[rs] < 0) incr_pc(offs << 2);
	else incr_pc(4);
}

void blez(reg rs, int16_t offs) {
	if (registers[rs] <= 0) incr_pc(offs << 2);
	else incr_pc(4);
}

void bgezal(reg rs, int16_t offs) {
	if (registers[rs] >= 0) {
		registers[31] = pc + 8;
		incr_pc(offs << 2);
	}else incr_pc(4);
}

void bltzal(reg rs, int16_t offs) {
	if (registers[rs] < 0) {
		registers[31] = pc + 8;
		incr_pc(offs << 2);
	}else incr_pc(4);
}

// Loads and stores
void lui(reg rt, uint32_t imm) {
	registers[rt] = imm << 16;
	incr_pc(4);
}

void lw(reg rt, reg rs, int32_t offset) {
	registers[rt] = read_memory(registers[rs] + offset, true);
	incr_pc(4);
}

void lb(reg rt, reg rs, int32_t offset) {
	registers[rt] = read_memory(registers[rs] + offset, false);
	incr_pc(4);
}

void sw(reg rt, reg rs, int32_t offset) {
	write_memory(registers[rs] + offset, registers[rt], true);
	incr_pc(4);
}

void sb(reg rt, reg rs, int32_t offset) {
	write_memory(registers[rs] + offset, registers[rt], false);
	incr_pc(4);
}

// Misc
void syscall() {
	incr_pc(4);
}

void mfhi(reg rd) {
	registers[rd] = hi;
	incr_pc(4);
}

void mflo(reg rd) {
	registers[rd] = lo;
	incr_pc(4);
}

void mthi(reg rd) {
	hi = registers[rd];
	incr_pc(4);
}

void mtlo(reg rd) {
	lo = registers[rd];
	incr_pc(4);
}

void slt(reg rd, reg rs, reg rt) {
	registers[rd] = (signed int) registers[rs] < (signed int) registers[rt];
	incr_pc(4);
}

void sltu(reg rd, reg rs, reg rt) {
	registers[rd] = registers[rs] < registers[rt];
	incr_pc(4);
}

void slti(reg rd, reg rs, int32_t imm) {
	registers[rd] = (signed int) registers[rs] < imm;
	incr_pc(4);
}

void sltiu(reg rd, reg rs, uint32_t imm) {
	registers[rd] = (signed int) registers[rs] < imm;
	incr_pc(4);
}
