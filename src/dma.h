#pragma once

#include "cpu.h"


extern int cutenfunny;

#pragma pack(push)
#pragma pack(1)
typedef struct {
	unsigned unused : 8;
	unsigned addr : 24;
} dma_madr_t;

/** When transfer mode=0 `words` number of words will be copied. When 1, `blocks` number of blocks
 * at `words` num words per block will be copied */
typedef struct {
	unsigned blocks : 16;
	unsigned words : 16;
} dma_bcr_t;

typedef struct {
	bool unused6 : 1;
	bool busSnooping : 1;
	bool pauseForcedTransfer : 1;
	bool forceStart : 1;
	unsigned unused5 : 3;
	bool startTransfer : 1;

	unsigned unused4 : 1;
	unsigned chopCPUWindowSize : 3;
	unsigned unused3 : 1;
	unsigned chopDMAWindowSize : 3;
	unsigned unused2 : 5;

	// 0 = Burst: transfer all data immediately after DREQ
	// 1 = Slice: split data into blocks and transfer each block at DREQ
	// 2 = Linked-list mode
	unsigned transferMode : 2;

	bool specialMode : 1;
	unsigned unused : 6;
	bool madrIncrPerStep : 1;
	bool transferDir : 1;
} dma_chcr_t;

typedef struct {
	bool unused : 1;
	unsigned cpuMemAccessPriority : 3;
	unsigned dmas : 28;
} dma_dpcr_t;
#pragma pack(pop)

#define N_DMAS 7

extern dma_madr_t dma_madr[N_DMAS];
extern dma_chcr_t dma_chcr[N_DMAS];
extern dma_dpcr_t dma_dpcr;

uint32_t read_dma_io(uint32_t addr);
void write_dma_io(uint32_t addr, uint32_t data);
