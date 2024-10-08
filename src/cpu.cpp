#include <cstdint>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "cpu.h"
#include "common.h"
#include "instructions.cpp"

/*
 * Missing:
 * sign extend immediates
 * handle overflow on arithmatic (traps)
 * jump delay slot
 * load delay slot
 */

char* ram;
char* scratchpad;
char* biosData;

uint32_t pc, npc;
uint32_t hi, lo;
uint32_t registers[N_REGISTERS];
uint32_t status;

void print_state();

void init() {
	ram = (char*) calloc(MEM_SIZE, sizeof(char));
	scratchpad = (char*) calloc(SCRATCH_SIZE, sizeof(char));
	biosData = (char*) calloc(BIOS_SIZE, sizeof(char));

	memset(registers, 0, N_REGISTERS * sizeof(uint32_t));
	hi = lo = status = 0;
	pc = 0x1FC00000;
	npc = pc + 4;

	char biosFile[] = "./SCPH-5501.BIN";
	FILE* bios = fopen(biosFile, "rn");
	if (!bios) {
		fprintf(stderr, "Failed to open bios rom: %s\n", biosFile);
		exit(-1);
	}

	int biosSize = fread(biosData, 1, BIOS_SIZE, bios);
	printf("Read bios file of %d bytes\n", biosSize);
	fclose(bios);
}

void incr_pc(size_t incr) {
	pc = npc;
	npc += incr;
}

uint32_t read_memory(uint32_t address, int num_bytes) {
	uint32_t mask = (unsigned) ~0 >> (32 - 8 * num_bytes);

	int mode = -1;
	if (address >> 24 == 0x00 || address >> 24 == 0x1F) mode = 0;
	if (address >> 24 == 0x80 || address >> 24 == 0x9F) mode = 1;
	if (address >> 24 == 0xA0 || address >> 24 == 0xBF) mode = 2;
	if (mode == -1) return 0;

	// Handle physical ram access
	if ((address & 0xFFFFFF) < MEM_SIZE) {
		return *(uint32_t*)(ram + (address & 0x1FFFFF)) & mask;

	// Scratchpad
	}else if ((address & 0xFFFFFF) >= 0x800000 && (address & 0xFFFFFF) < 0x800400) {
		return *(uint32_t*)(scratchpad + (address & 0x3FF)) & mask;

	// BIOS
	}else if ((address & 0xFFFFFF) >= 0xC00000 && (address & 0xFFFFFF) < 0xC80000) {
		return *(uint32_t*)(biosData + (address & 0x7FFFF)) & mask;
	}

	printf("uncaught read oooh noooooo %#x\n", address);

	return 0;
}

void write_memory(uint32_t address, uint32_t data, int num_bytes) {
	int mode = -1;
	if (address >> 24 == 0x00 || address >> 24 == 0x1F) mode = 0;
	if (address >> 24 == 0x80 || address >> 24 == 0x9F) mode = 1;
	if (address >> 24 == 0xA0 || address >> 24 == 0xBF) mode = 2;
	if (mode == -1) return;

	// Handle physical ram access
	uint32_t local_addr = (address & 0xFFFFFF);
	if (local_addr < MEM_SIZE) {
		if (num_bytes == 4)
			*(uint32_t *)(ram + (address & 0x1FFFFF)) = data;
		else if (num_bytes == 2)
			*(uint16_t *)(ram + (address & 0x1FFFFF)) = data;
		else if (num_bytes == 1)
			ram[address & 0x1FFFFF] = data;

	// Scratchpad
	}else if (local_addr >= 0x800000 && local_addr < 0x800400) {
		if (num_bytes == 4)
			*(uint32_t *)(scratchpad + (address & 0x3FF)) = data;
		if (num_bytes == 2)
			*(uint16_t *)(scratchpad + (address & 0x3FF)) = data;
		else if (num_bytes == 1)
			scratchpad[address & 0x3FF] = data;

	// BIOS
	}else if (local_addr >= 0xC00000 && local_addr < 0xC80000) {
		if (num_bytes == 4)
			*(uint32_t *)(biosData + (address & 0x7FFFF)) = data;
		if (num_bytes == 2)
			*(uint16_t *)(biosData + (address & 0x7FFFF)) = data;
		else if (num_bytes == 1)
			biosData[address & 0x7FFFF] = data;
	}else {
		printf("uncaught write oooh noooooo %#X, data=%#X (%d)\n", address, data, data);
	}
}

int tick() {
	// uint32_t instruction = ((uint32_t*) ram)[pc >> 2];
	uint32_t instruction = read_memory(pc, 4);
	// print_word("pc", pc);
	// print_word("executing instruction", instruction);
	// General
	uint8_t opcode = (instruction >> 26) & 0b111111;

	// R/I instructions
	size_t rs = (instruction >> 21) & 0b11111;
	size_t rt = (instruction >> 16) & 0b11111;

	// R instructions
	size_t rd = (instruction >> 11) & 0b11111;
	size_t shamt = (instruction >> 6) & 0b11111;
	size_t funct = instruction & 0b111111;

	// I instructions
	int16_t immediate_s = instruction & 0xFFFF;
	uint16_t immediate_u = instruction & 0xFFFF;

	// J instructions
	uint32_t address = instruction & 0x0003FFFFFF;

	// Co-proc instructions
	char coproc = opcode & 0b11;

	print_instruction(pc);

	switch (opcode) {
		case 0b000000: // funct instructions
			switch (funct) {
			case 0b000000:
				// NOOP :D
				// clashes with sll
				sll(rd, rt, shamt);
				break;
			case 0b000010:
				srl(rd, rt, shamt);
				break;
			case 0b000011:
				sra(rd, rt, shamt);
				break;
			case 0b000100:
				sllv(rd, rt, rs);
				break;
			case 0b000110:
				srlv(rd, rt, rs);
				break;
			case 0b001000:
				jr(rs);
				break;
			case 0b001100:
				syscall();
				break;
			case 0b010000:
				mfhi(rd);
				break;
			case 0b010010:
				mflo(rd);
				break;
			case 0b011000:
				mult(rs, rt);
				break;
			case 0b011001:
				multu(rs, rt);
				break;
			case 0b011010:
				div(rs, rt);
				break;
			case 0b011011:
				divu(rs, rt);
				break;
			case 0b100000:
				add(rd, rs, rt);
				break;
			case 0b100001:
				addu(rd, rs, rt);
				break;
			case 0b100010:
				sub(rd, rs, rt);
				break;
			case 0b100011:
				subu(rd, rs, rt);
				break;
			case 0b100100:
				andd(rd, rs, rt);
				break;
			case 0b100101:
				orr(rd, rs, rt);
				break;
			case 0b100110:
				xorr(rd, rs, rt);
				break;
			case 0b101010:
				slt(rd, rs, rt);
				break;
			case 0b101011:
				sltu(rd, rs, rt);
				break;
			default:
				incr_pc(4);
			}
			break;
		case 0b000010:
			j(address);
			break;
		case 0b000011:
			jal(address);
			break;
		case 0b001000:
			addi(rt, rs, immediate_s);
			break;
		case 0b001001:
			addiu(rt, rs, immediate_s);
			break;
		case 0b001100:
			andi(rt, rs, immediate_u);
			break;
		case 0b001110:
			xori(rt, rs, immediate_u);
			break;
		case 0b000100:
			beq(rs, rt, immediate_s);
			break;
		case 0b000101:
			bne(rs, rt, immediate_s);
			break;
		case 0b000001:
			switch (rt) {
			case 0b00000:
				bltz(rs, immediate_s);
				break;
			case 0b00001:
				bgez(rs, immediate_s);
				break;
			case 0b10000:
				bltzal(rs, immediate_s);
				break;
			case 0b10001:
				bgezal(rs, immediate_s);
				break;
			default:
				incr_pc(4);
			}
			break;
		case 0b000111:
			switch (rt) {
			case 0b00000:
				bgtz(rs, immediate_s);
				break;
			default:
				incr_pc(4);
			}
			break;
		case 0b000110:
			switch (rt) {
			case 0b00000:
				blez(rs, immediate_s);
				break;
			default:
				incr_pc(4);
			}
			break;
		case 0b001010:
			slti(rt, rs, immediate_s);
			break;
		case 0b001011:
			sltiu(rt, rs, immediate_u);
			break;
		case 0b001101:
			ori(rt, rs, immediate_s);
			break;
		case 0b001111:
			lui(rt, immediate_s);
			break;
		case 0b010000:
		case 0b010001:
		case 0b010010:
		case 0b010011:
			if (rs == 0b00100) mtc(coproc, rt, rd);
			else if (rs == 0b00000) mfc(coproc, rt, rd);
			else print_word("uncaught coproc instruction", instruction);
			incr_pc(4);
			break;
		case 0b100000:
			lb(rt, rs, immediate_s);
			break;
		case 0b100001:
			lh(rt, rs, immediate_s);
			break;
		case 0b100011:
			lw(rt, rs, immediate_s);
			break;
		case 0b101000:
			sb(rt, rs, immediate_s);
			break;
		case 0b101001:
			sh(rt, rs, immediate_s);
			break;
		case 0b101011:
			sw(rt, rs, immediate_s);
			break;
		default:
			print_state();
			print_word("Unknown instruction D:!", instruction);
			const char* mnemonic = get_mnemonic(instruction, opcode);
			printf("%#x\t%#x\tmnemonic: %s %zu, %zu, %zu\n", pc, opcode, mnemonic, rs, rt, rd);
			incr_pc(4);
			return false;
	}

	// r0 is read-only but that's annoying to implement, just set it to 0 every tick
	registers[0] = 0;

	// print_state();
	return true;
}

void destroy() {
	free(ram);
}

void print_state() {
	printf("-------------------------- CPU STATUS -------------------------\n");
	for (int i = 0; i < N_REGISTERS; i++) {
		int r = (i % 8) * 4 + i / 8;
		printf("r%-2d = %d\t", r, registers[r]);
		if (i % 8 == 7) printf("\n");
	}
	printf("\nPC: %d\tnPC: %d\n", pc, npc);
}

int main() {
	init();

	uint32_t program[] = {
		// 0b001000'00001'00001'0000000000000001,
		// 0b000001'00001'00001'0000111100001111
		0b001110'00000'00010'0000000000000000 | 'f', // xor r2, r0, 'f'
		0b101000'00000'00010'0000000000000000 | 100, // sb r2, 100(r0)
		0b001110'00000'00010'0000000000000000 | 'u',
		0b101000'00000'00010'0000000000000000 | 101,
		0b001110'00000'00010'0000000000000000 | 'n',
		0b101000'00000'00010'0000000000000000 | 102,
		0b001110'00000'00010'0000000000000000 | 'n',
		0b101000'00000'00010'0000000000000000 | 103,
		0b001110'00000'00010'0000000000000000 | 'y',
		0b101000'00000'00010'0000000000000000 | 104,
	};

	memcpy(ram, program, sizeof(program) / sizeof(uint32_t));
	for (int i = 0; i < sizeof(program) / sizeof(uint32_t); i++)
		((uint32_t*)ram)[i] = (program[i]);

	int i;
	for (i = 0; i < 0x4400; i++) {
		if (tick() == false) break;
	}
	printf("%d instructions executed\n", i + 1);

	// printf("memory[100] = %s\n", ram + 100);

	print_state();
	printf("size of ins_r: %zu\n", sizeof(ins_r));
	printf("size of ins_i: %zu\n", sizeof(ins_i));
	printf("size of ins_j: %zu\n", sizeof(ins_j));

	destroy();
	return 0;
}
