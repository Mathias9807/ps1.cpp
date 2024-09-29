#pragma once

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>


extern const char* mnemonics[];
extern const char* secondary_mnems[];
extern const char* tertiary_mnems[];

uint32_t to_le(uint32_t);
uint32_t to_be(uint32_t);

const char* get_mnemonic(uint32_t instruction, uint8_t funct);

void print_word(const char* label, uint32_t i);
void print_instruction(uint32_t address);
