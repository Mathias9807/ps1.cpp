#include "cpu.h"


void returnFromFunc() {
	pc = registers[31];
	npc = pc + 4;
}

/** Some slow kernel functions are much faster to run on host than in interpreted mips,
 * catch calls and run in compiled code */
bool try_xlr8() {
	// memcpy, dest=r4, src=r5, data_len=r6, returns null or dest pointer into r2
	if ((pc & 0xFFFFFF) == 0xC02B50) {
		printf("Running native memcpy implementation, dest=%#x, src=%#x, len=%d\n",
				registers[4], registers[5], registers[6]);
		if (registers[4] == 0) { // If (dest == NULL) return NULL;
			registers[2] = 0;
			returnFromFunc();
			return true;
		}

		registers[3] = registers[4];
		while (registers[6] > 0) {
			registers[14] = read_memory(registers[5], 1);
			write_memory(registers[4], registers[14], 1);
			registers[4]++;
			registers[5]++;
			registers[6]--;
		}

		registers[2] = registers[3];
		returnFromFunc();
		return true;
	}

	return false;
}
