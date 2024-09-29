#include "common.h"
#include "cpu.h"

#include <cstdint>
#include <stdio.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')


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

uint32_t to_le(uint32_t i) {
	return i;
}

uint32_t to_be(uint32_t i) {
	return __builtin_bswap32(i);
}

void print_word(const char* label, uint32_t i) {
	printf("WORD %s: 0b" BYTE_TO_BINARY_PATTERN BYTE_TO_BINARY_PATTERN BYTE_TO_BINARY_PATTERN BYTE_TO_BINARY_PATTERN"\n", label, BYTE_TO_BINARY(i >> 24), BYTE_TO_BINARY(i >> 16), BYTE_TO_BINARY(i >> 8), BYTE_TO_BINARY(i));
}

const char* get_mnemonic(uint32_t instruction, uint8_t funct) {
	uint8_t opcode = (instruction >> 26) & 0b111111;
	size_t rt = (instruction >> 16) & 0b11111;
	const char* mnemonic = mnemonics[opcode];
	if (opcode == 0) mnemonic = secondary_mnems[funct];
	if (opcode == 1) mnemonic = tertiary_mnems[rt >> 4 | (rt & 1)];
	if (!instruction) mnemonic = "nop";
	return mnemonic;
}

void print_instruction(uint32_t address) {
	uint32_t instruction = read_memory(pc, 4);
	uint8_t opcode = (instruction >> 26) & 0b111111;
	uint8_t funct = instruction & 0b111111;
	ins_r ir = *(ins_r*) &instruction;
	ins_i ii = *(ins_i*) &instruction;
	ins_j ij = *(ins_j*) &instruction;

	const char* mnemonic = get_mnemonic(instruction, funct);

	if (!instruction) {
		printf("%#x\t%s\n", pc, mnemonic);
		return;
	}

	if (opcode == 0) {
		switch (funct) {
		case 0b100000: case 0b100001: case 0b100100: case 0b100111: case 0b100101:
		case 0b101010: case 0b101011: case 0b100010: case 0b100011: case 0b100110: // <ins> rd rs rt instructions
			printf("%#x\t%s r%d, r%d, r%d\n", pc, mnemonic, ir.rd, ir.rs, ir.rt);
			return;
		case 0b001000: // <ins> rs instructions
			printf("%#x\t%s r%d\n", pc, mnemonic, ir.rs);
			return;
		}
	}
	uint32_t next, prev; ins_i next_ins, prev_ins;
	uint32_t imm;
	switch (opcode) {
		case 0b000010:
		case 0b000011:
			printf("%#x\t%s %#x\n", pc, mnemonic, ij.address << 2);
			return;
		case 0b001111:

			// Handle `li` macro
			next = read_memory(pc + 4, 4);
			next_ins = *(ins_i*) &next;
			if (next_ins.opcode == 0b001101) {
				imm = (ii.immediate << 16) | next_ins.immediate;
				printf("%#x\t%s r%d, %#x (%d)\n", pc, "li", ii.rt, imm, imm);
			}else
				printf("%#x\t%s r%d, %#x\n", pc, mnemonic, ii.rt, ii.immediate);
			return;
		case 0b001101:
		case 0b001001:
			if (opcode == 0b001101) {
				// Don't print anything if in `li` macro
				prev = read_memory(pc - 4, 4);
				prev_ins = *(ins_i*) &prev;
				if (prev_ins.opcode == 0b001111)
					printf("%#x\n", pc);
				else
					printf("%#x\t%s r%d, r%d, %#x\n", pc, mnemonic, ii.rt, ii.rs, ii.immediate);
			}else if (opcode == 0b001001) // addiu
				printf("%#x\t%s r%d, r%d, %#x (%d)\n", pc, mnemonic, ii.rt, ii.rs, ii.immediate, (int16_t) ii.immediate);
			else
				printf("%#x\t%s r%d, r%d, %#x\n", pc, mnemonic, ii.rt, ii.rs, ii.immediate);
			return;

		// Address offset store/loads
		case 0b100000: case 0b100100: case 0b100001: case 0b100101:
		case 0b100011: case 0b101011: case 0b101001: case 0b101000:
			printf("%#x\t%s r%d, %#x(r%d) = %#x\n", pc, mnemonic, ii.rt, ii.immediate, ii.rs, ii.immediate + registers[ii.rs]);
			return;
	}
	printf("%#x\t%s (r%d, r%d, r%d)\n", pc, mnemonic, ir.rs, ir.rt, ir.rd);
}
