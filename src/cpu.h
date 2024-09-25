#pragma once

#include "common.h"

typedef size_t reg;

#define MEM_SIZE (2 * 1024 * 1024)
#define SCRATCH_SIZE (1024)
#define N_REGISTERS 32

#define BIOS_SIZE (512 * 1024)

extern uint32_t pc, npc;
extern uint32_t hi, lo;
extern uint32_t registers[N_REGISTERS];

void incr_pc(size_t incr);

uint32_t read_memory(uint32_t address, bool read_word);
void write_memory(uint32_t address, uint32_t data, bool write_word);
