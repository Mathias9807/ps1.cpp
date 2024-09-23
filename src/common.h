#pragma once

#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

uint32_t to_le(uint32_t);
uint32_t to_be(uint32_t);

void print_word(const char* label, uint32_t i);
