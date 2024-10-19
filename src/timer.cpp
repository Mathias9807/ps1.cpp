#include "cpu.h"

#include <stdlib.h>

uint32_t timer_counter[3];
timer_config timer_mode[3];
uint32_t timer_target[3];

// Interrupts occur when timer_mode[t].irqReq goes from high to low

void setIrqReq(int timer, int req) {
	if (req == 0 && timer_mode[timer].irqReq == 1) {
		// Interrupt!
	}

	timer_mode[timer].irqReq = req;
}

void irqReq(int timer) {
	// If IRQ Once and we've already fired, return
	if (timer_mode[timer].irqRepeat == false && timer_mode[timer].hasFired)
		return;

	if (timer_mode[timer].irqToggleMode == 1) { // Toggle
		setIrqReq(timer, !timer_mode[timer].irqReq);
	}else { // Pulse
		setIrqReq(timer, 0);
		setIrqReq(timer, 1);
	}
	timer_mode[timer].hasFired = true;
}

void on_tick() {
	for (int t = 0; t < 3; t++) {
		// No synchronization with VBlanks or so
		if (timer_mode[t].enable == 0)
			timer_counter[t]++;

		// Trigger irq
		if (timer_mode[t].irqOnOF && timer_counter[t] == 0xFFFF) {
			timer_mode[t].reachedOF = 1;
			irqReq(t);
		}
		if (timer_mode[t].irqOnTarget && (uint16_t) timer_counter[t] == (uint16_t) timer_target[t]) {
			timer_mode[t].reachedTarget = 1;
			irqReq(t);
		}

		// Reset on OF or clock=target
		if (timer_mode[t].resetOnTarget == 0)
			timer_counter[t] &= 0xFFFF;
		else { // TIMER_FLAG_RESET == 1
			if ((uint16_t) timer_counter[t] > (uint16_t)
					timer_target[t]) timer_counter[t] = 0;
		}
	}
}

void set_timer_mode(int timer, uint32_t flags) {
	timer_counter[timer] = 0; // Force reset timer when mode set

	*(uint32_t*) &timer_mode[timer] = flags;

	printf("set_timer_mode timer %d, flags %#x\n", timer, flags);
	if (timer_mode[timer].enable) {
		printf("Attempt to set timer %d synchronize, unsupported!\n", timer);
		exit(-1);
	}

	if (timer_mode[timer].clockSrc != 0) {
		printf("Attempt to set timer %d clock source to something other than system clock, unsupported!\n", timer);
		exit(-1);
	}

	// Interrupt request is set to 1 (no) on every mode set
	timer_mode[timer].irqReq = 1;

	timer_mode[timer].hasFired = 0;
}

uint32_t get_timer_mode(int timer) {
	uint32_t mode = *(uint32_t*) &timer_mode[timer];

	// Clear after read
	timer_mode[timer].reachedOF = 0;
	timer_mode[timer].reachedTarget = 0;

	return mode;
}
