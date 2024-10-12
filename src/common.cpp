#include "common.h"
#include "cpu.h"
#include "kernel_functions.h"

#include <cstdint>
#include <stdio.h>
#include <stdlib.h>

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

size_t* num_accesses = 0, * num_secondary_accesses = 0, * num_tertiary_accesses = 0;

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
	if (!num_accesses) {
		num_accesses = (size_t*) calloc(sizeof(mnemonics), sizeof(size_t));
		num_secondary_accesses = (size_t*) calloc(sizeof(secondary_mnems), sizeof(size_t));
		num_tertiary_accesses = (size_t*) calloc(sizeof(tertiary_mnems), sizeof(size_t));
	}

	uint32_t instruction = read_memory(pc, 4);
	uint8_t opcode = (instruction >> 26) & 0b111111;
	uint8_t funct = instruction & 0b111111;
	ins_r ir = *(ins_r*) &instruction;
	ins_i ii = *(ins_i*) &instruction;
	ins_j ij = *(ins_j*) &instruction;

	if (opcode == 0) num_secondary_accesses[funct]++;
	else if (opcode == 1) num_tertiary_accesses[ir.rt >> 4 | (ir.rt & 1)]++;
	else num_accesses[opcode]++;

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
		case 0b000000: // <ins> rd rt sa instructions
			printf("%#x\t%s r%d, r%d, %d\n", pc, mnemonic, ir.rd, ir.rt, ir.shamt);
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
		case 0b001010:
		case 0b001011:
			printf("%#x\t%s r%d, r%d (%d), %#x (%d)\n", pc, mnemonic, ii.rt, ii.rs, registers[ii.rs], ii.immediate, ii.immediate);
			return;
		case 0b000001:
		case 0b000110:
		case 0b000111:
			printf("%#x\t%s r%d (%d), %#x\n", pc, mnemonic, ii.rs, registers[ii.rs], pc + (int16_t) ii.immediate*4);
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
				printf("%#x\t%s r%d, r%d (%d), %#x (%d)\n", pc, mnemonic, ii.rt, ii.rs, registers[ii.rs], ii.immediate, (int16_t) ii.immediate);
			else
				printf("%#x\t%s r%d, r%d, %#x\n", pc, mnemonic, ii.rt, ii.rs, ii.immediate);
			return;

		// Co-processor instructions
		case 0b010000: case 0b010001: case 0b010010: case 0b010011:
			if (ir.rs == 0b00100)
				printf("%#x\tmtc coproc%dr%d, r%d (%#x)\n", pc, opcode & 0b11, ir.rd, ir.rt, registers[ir.rt]);
			else if (!ir.rs)
				printf("%#x\tmfc coproc%dr%d, r%d\n", pc, opcode & 0b11, ir.rd, ir.rt);
			else {
				printf("Unknown coprocessor instruction!!\n");
				exit(-1);
			}
			return;

		// Address offset store/loads
		case 0b100000: case 0b100100: case 0b100001: case 0b100101:
		case 0b100011: case 0b101011: case 0b101001: case 0b101000:
			printf("%#x\t%s r%d, %#x(r%d) = %#x\n", pc, mnemonic, ii.rt, ii.immediate, ii.rs, (int16_t) ii.immediate + registers[ii.rs]);
			return;
	}
	printf("%#x\t%s (r%d, r%d, r%d)\n", pc, mnemonic, ir.rs, ir.rt, ir.rd);
}

void print_kernel_call() {
	if (pc == 0xA0) {
		kernel_func_t func = kernel_a_funcs[registers[9]];

		printf("Kernel call: %s(", func.func_name);
		if (func.param_1) printf("%s", func.param_1);

		uint32_t p_1 = registers[4];
		uint32_t p_2 = registers[5];
		if (func.param_1_type == KERNEL_TYPE_STRING && find_memory(p_1)) printf("=\"%s\"", (char*) find_memory(p_1));
		if (func.param_2) printf(", %s", func.param_2);
		if (func.param_2_type == KERNEL_TYPE_STRING && find_memory(p_2)) printf("=\"%s\"", (char*) find_memory(p_2));
		if (func.param_3) printf(", %s", func.param_3);
		if (func.param_4) printf(", %s", func.param_4);
		if (func.param_5) printf(", %s", func.param_5);
		printf(")\n");
	}

	if (pc == 0xB0) {
		kernel_func_t func = kernel_b_funcs[registers[9]];

		printf("Kernel call: %s(", func.func_name);
		if (func.param_1) printf("%s", func.param_1);

		uint32_t p_1 = registers[4];
		uint32_t p_2 = registers[5];
		if (func.param_1_type == KERNEL_TYPE_STRING && find_memory(p_1)) printf("=\"%s\"", (char*) find_memory(p_1));
		if (func.param_1_type == KERNEL_TYPE_CHAR) printf("=\"%c\"", p_1);
		if (func.param_2) printf(", %s", func.param_2);
		if (func.param_2_type == KERNEL_TYPE_STRING && find_memory(p_2)) printf("=\"%s\"", (char*) find_memory(p_2));
		if (func.param_3) printf(", %s", func.param_3);
		if (func.param_4) printf(", %s", func.param_4);
		if (func.param_5) printf(", %s", func.param_5);
		printf(")\n");
	}
}

void common_exit() {
	if (!num_accesses) return;

	printf("Num accesses:\n");
	for (int i = 0; i < sizeof(mnemonics); i++) {
		if (!num_accesses[i]) continue;
		printf("\topcode: %#x, %s: %ld\n", i, mnemonics[i], num_accesses[i]);
	}
	free(num_accesses);

	for (int i = 0; i < sizeof(secondary_mnems); i++) {
		if (!num_secondary_accesses[i]) continue;
		printf("\topcode: %#x, %s: %ld\n", i, secondary_mnems[i], num_secondary_accesses[i]);
	}
	free(num_secondary_accesses);

	for (int i = 0; i < sizeof(tertiary_mnems); i++) {
		if (!num_tertiary_accesses[i]) continue;
		printf("\topcode: %#x, %s: %ld\n", i, tertiary_mnems[i], num_tertiary_accesses[i]);
	}
	free(num_tertiary_accesses);
}

