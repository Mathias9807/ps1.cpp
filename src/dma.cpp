#include "dma.h"
#include "cpu.h"
#include <stdlib.h>


dma_madr_t dma_madr[N_DMAS];
dma_chcr_t dma_chcr[N_DMAS];
dma_dpcr_t dma_dpcr;

void set_dma_dpcr(dma_dpcr_t new_dpcr) {
	dma_dpcr = new_dpcr;

	for (int i = 0; i < N_DMAS; i++) {
		if (dma_dpcr.dmas >> (3 * i) & 0b100) {
			printf("set_dma_dpcr: DMA%d not implemented\n", i);
			quit = 1;
		}
	}
}

uint32_t read_dma_io(uint32_t addr) {
	uint32_t local_addr = addr & 0xFFFFFF;
	if (local_addr < 801080 || local_addr >= 0x8010F8) {
		printf("read_dma_io: called with invalid address %#x\n", addr);
		quit = 1;
	}

	if (local_addr == 0x8010F0) {
		return *(uint32_t*) &dma_dpcr;

	}else {
		printf("read_dma_io: mmapped DMA register at address %#x not implemented yet\n", addr);
		quit = 1;
	}

	return 0;
}

void write_dma_io(uint32_t addr, uint32_t data) {
	uint32_t local_addr = addr & 0xFFFFFF;
	if (local_addr < 801080 || local_addr >= 0x8010F8) {
		printf("write_dma_io: called with invalid address %#x\n", addr);
		quit = 1;
	}

	if (local_addr == 0x8010F0) {
		set_dma_dpcr(*(dma_dpcr_t*) &data);

	}else {
		printf("write_dma_io: mmapped DMA register at address %#x not implemented yet\n", addr);
		quit = 1;
	}
}
