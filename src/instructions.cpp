#pragma once

#include "cpu.h"
#include <cstdint>
#include <stdlib.h>


// Math instructions
void add(reg rd, reg rs, reg rt) {
	registers[rd] = registers[rs] + registers[rt];
	incr_pc(4);
}

void addi(reg rt, reg rs, int32_t imm) {
	registers[rt] = registers[rs] + imm;
	incr_pc(4);
}

void addiu(reg rt, reg rs, int16_t imm) {
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
	registers[rd] = registers[rt] << shamt;
	incr_pc(4);
}

void sllv(reg rd, reg rt, reg rs) {
	registers[rd] = registers[rt] << registers[rs];
	incr_pc(4);
}

// ok fuck. signed right shifts in c are UB, not necessarily arithmetic
void sra(reg rd, reg rt, uint16_t shamt) {
	registers[rd] = (int32_t) registers[rt] >> (int32_t) shamt;
	incr_pc(4);
}

void srl(reg rd, reg rt, uint16_t shamt) {
	registers[rd] = registers[rt] >> shamt;
	incr_pc(4);
}

void srlv(reg rd, reg rt, reg rs) {
	registers[rd] = registers[rt] >> registers[rs];
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
	if ((int32_t) registers[rs] >= 0) incr_pc(offs << 2);
	else incr_pc(4);
}

void bgtz(reg rs, int16_t offs) {
	if ((int32_t) registers[rs] > 0) incr_pc(offs << 2);
	else incr_pc(4);
}

void bltz(reg rs, int16_t offs) {
	if ((int32_t) registers[rs] < 0) incr_pc(offs << 2);
	else incr_pc(4);
}

void blez(reg rs, int16_t offs) {
	if ((int32_t) registers[rs] <= 0) incr_pc(offs << 2);
	else incr_pc(4);
}

void bgezal(reg rs, int16_t offs) {
	if ((int32_t) registers[rs] >= 0) {
		registers[31] = pc + 8;
		incr_pc(offs << 2);
	}else incr_pc(4);
}

void bltzal(reg rs, int16_t offs) {
	if ((int32_t) registers[rs] < 0) {
		registers[31] = pc + 8;
		incr_pc(offs << 2);
	}else incr_pc(4);
}

// Loads and stores
void lui(reg rt, uint32_t imm) {
	registers[rt] = imm << 16;
	incr_pc(4);
}

void lw(reg rt, reg rs, int16_t offset) {
	// Ignore all reads when Isolate Cache is high
	if ((status & (1 << 16)) == 0)
		registers[rt] = read_memory(registers[rs] + offset, 4);
	incr_pc(4);
}

void lh(reg rt, reg rs, int16_t offset) {
	// Ignore all reads when Isolate Cache is high
	if ((status & (1 << 16)) == 0)
		registers[rt] = (int16_t) read_memory(registers[rs] + offset, 2);
	incr_pc(4);
}

void lb(reg rt, reg rs, int16_t offset) {
	// Ignore all reads when Isolate Cache is high
	if ((status & (1 << 16)) == 0)
		registers[rt] = (int8_t) read_memory(registers[rs] + offset, 1);
	incr_pc(4);
}

void lbu(reg rt, reg rs, int16_t offset) {
	// Ignore all reads when Isolate Cache is high
	if ((status & (1 << 16)) == 0)
		registers[rt] = (uint8_t) read_memory(registers[rs] + offset, 1);
	incr_pc(4);
}

void sw(reg rt, reg rs, int16_t offset) {
	// Ignore all writes when Isolate Cache is high
	if ((status & (1 << 16)) == 0)
		write_memory(registers[rs] + offset, registers[rt], 4);
	incr_pc(4);
}

void sh(reg rt, reg rs, int32_t offset) {
	// Ignore all writes when Isolate Cache is high
	if ((status & (1 << 16)) == 0)
		write_memory(registers[rs] + offset, registers[rt], 2);
	incr_pc(4);
}

void sb(reg rt, reg rs, int32_t offset) {
	// Ignore all writes when Isolate Cache is high
	if ((status & (1 << 16)) == 0)
		write_memory(registers[rs] + offset, registers[rt], 1);
	incr_pc(4);
}

// Misc
void syscall() {
	cause = (cause & ~0b11111100) | (8 << 2);
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

void slti(reg rd, reg rs, int16_t imm) {
	registers[rd] = (int32_t) registers[rs] < imm;
	incr_pc(4);
}

void sltiu(reg rd, reg rs, int16_t imm) {
	registers[rd] = (uint32_t) registers[rs] < imm;
	incr_pc(4);
}

// Co-processor instructions

void mtc(char coproc, reg rt, reg rd) {
	if (coproc == 0 && rd == 12) {
		status = registers[rt];

	}else if (coproc == 0 && rd == 7) {
		if (registers[rt]) {
			printf("Write to coproc 0, DCIC, data %#x\n", registers[rt]);
			printf("Haven't implemented breakpoints!!\n");
			exit(-1);
		}
		dcic = registers[rt];
	}else if (coproc == 0 && rd == 3) {
		if (registers[rt]) {
			printf("Write to coproc 0, BPC, data %#x\n", registers[rt]);
			printf("Haven't implemented breakpoints!!\n");
			exit(-1);
		}
		bpc = registers[rt];
	}else if (coproc == 0 && rd == 5) {
		bda = registers[rt];
	}else if (coproc == 0 && rd == 9) {
		bdam = registers[rt];
	}else if (coproc == 0 && rd == 3) {
		bpc = registers[rt];
	}else if (coproc == 0 && rd == 11) {
		bpcm = registers[rt];
	}else if (coproc == 0 && rd == 13) {
		cause = (cause & 0b110000000) | (registers[rt] & 0b110000000);
	}else if (coproc == 0 && rd == 6) {
		printf("Write to read-only register 6 of coproc 0, data %#x\n", registers[rt]);

	}else {
		printf("Uncaught write to coproc %d, register %zu, data %#x\n", coproc, rd, registers[rt]);
		exit(-1);
	}
}

void mfc(char coproc, reg rt, reg rd) {
	if (coproc == 0 && rd == 12) {
		printf("Read from status register\n");
		registers[rt] = status;
	}else if (coproc == 0 && rd == 7) {
		registers[rt] = dcic;
	}else if (coproc == 0 && rd == 3) {
		registers[rt] = bpc;
	}else if (coproc == 0 && rd == 5) {
		registers[rt] = bda;
	}else if (coproc == 0 && rd == 13) {
		registers[rt] = bda;
	}else {
		printf("Uncaught read from coproc %d, register %zu\n", coproc, rd);
		exit(-1);
	}
}
