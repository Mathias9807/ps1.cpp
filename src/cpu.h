#pragma once

#include "common.h"
#include "dma.h"
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

typedef struct {
	bool iec : 1;
	bool kuc : 1;
	bool iep : 1;
	bool kup : 1;
	bool ieo : 1;
	bool kuo : 1;
	unsigned unused : 2;
	unsigned im : 8;
	bool isc : 1;
	bool swc : 1;
	bool pz : 1;
	bool cm : 1;
	bool pe : 1;
	bool ts : 1;
	bool bev : 1;
	unsigned unused2 : 2;
	bool re : 1;
	unsigned unused3 : 2;
	bool cu0 : 1;
	bool cu1 : 1;
	bool cu2 : 1;
	bool cu3 : 1;
} sr_t;
typedef struct {
	unsigned unused : 2;
	unsigned excode : 5;

	bool unused2 : 1;
	unsigned ip : 8;

	unsigned unused3 : 12;
	unsigned ce : 2;

	bool unused4 : 1;
	bool bd : 1;
} cause_t;
#pragma pack(pop)

extern uint32_t pc, npc;
extern uint32_t hi, lo;
extern uint32_t registers[N_REGISTERS];
extern sr_t	status;
extern cause_t	cause;
extern uint32_t dcic, bpc, bda, bdam, bpc, bpcm, epc;
extern uint16_t volumeLeft, volumeRight, reverbVolumeLeft, reverbVolumeRight;
extern uint32_t irq_stat, irq_mask;

extern bool quit;

#define DCIC_BPC (1 << 24)
#define DCIC_BDA (1 << 25)

void incr_pc(size_t incr);

uint32_t read_memory(uint32_t address, int num_bytes);
void write_memory(uint32_t address, uint32_t data, int num_bytes);
void* find_memory(uint32_t address);
bool try_xlr8();
void print_state();

void exception();
void interrupt(int source);

#define IRQ_VBLANK	0
#define IRQ_GPU		1
#define IRQ_CDROM	2
#define IRQ_DMA		3
#define IRQ_TMR0	4
#define IRQ_TMR1	5
#define IRQ_TMR2	6
#define IRQ_CONTROLLER	7
#define IRQ_SIO		8
#define IRQ_SPU		9
#define IRQ_LIGHTPEN	10

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
