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

#pragma pack(push)
#pragma pack(1)
typedef struct {
	uint16_t	garbage;
	unsigned	other : 2;
	bool		hasFired : 1;
	bool		reachedOF : 1;
	bool		reachedTarget : 1;
	bool		irqReq : 1;
	unsigned	clockSrc : 2;
	bool		irqToggleMode : 1;
	bool		irqRepeat : 1;
	bool		irqOnOF : 1;
	bool		irqOnTarget : 1;
	bool		resetOnTarget : 1;
	unsigned	mode : 2;
	bool		enable : 1;
} timer_config;
#pragma pack(pop)

#define TIMER_FLAG_SYNC_ENABLE	0
#define TIMER_FLAG_SYNC_MODE	1
#define TIMER_FLAG_RESET	3
#define TIMER_IRQ_ON_TARGET	4
#define TIMER_IRQ_ON_OF		5
#define TIMER_IRQ_REPEAT	6
#define TIMER_IRQ_TOGGLE	7
#define TIMER_CLOCK_SRC		8
#define TIMER_IRQ_REQ		10
#define TIMER_REACHED_TARGET	11
#define TIMER_REACHED_OF	12

extern uint32_t timer_counter[3];
extern timer_config timer_mode[3];
extern uint32_t timer_target[3];

void incr_timers();
void on_tick();
void set_timer_mode(int timer, uint32_t flags);
uint32_t get_timer_mode(int timer);
