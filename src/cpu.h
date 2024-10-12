#pragma once

#include "common.h"
#include <cstdint>

typedef size_t reg;

#define MEM_SIZE (2 * 1024 * 1024)
#define SCRATCH_SIZE (1024)
#define N_REGISTERS 32

#define BIOS_SIZE (512 * 1024)

#define WORK_MEM_SIZE (8 * 1024 * 1024)

#pragma pack(push)
#pragma pack(1)
typedef struct {
	uint8_t funct : 6;
	uint8_t shamt : 5;
	uint8_t rd : 5;
	uint8_t rt : 5;
	uint8_t rs : 5;
	uint8_t opcode : 6;
} ins_r;
typedef struct {
	uint16_t immediate : 16;
	uint8_t rt : 5;
	uint8_t rs : 5;
	uint8_t opcode : 6;
} ins_i;
typedef struct {
	uint32_t address : 26;
	uint8_t opcode : 6;
} ins_j;
#pragma pack(pop)

extern uint32_t pc, npc;
extern uint32_t hi, lo;
extern uint32_t registers[N_REGISTERS];
extern uint32_t status, dcic, bpc, bda, bdam, bpc, bpcm, cause;
extern uint16_t volumeLeft, volumeRight, reverbVolumeLeft, reverbVolumeRight;
extern uint32_t irq_stat, irq_mask;

#define DCIC_BPC (1 << 24)
#define DCIC_BDA (1 << 25)

void incr_pc(size_t incr);

uint32_t read_memory(uint32_t address, int num_bytes);
void write_memory(uint32_t address, uint32_t data, int num_bytes);
void* find_memory(uint32_t address);
