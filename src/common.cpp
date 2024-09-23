#include "common.h"

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')


uint32_t to_le(uint32_t i) {
	return i;
}

uint32_t to_be(uint32_t i) {
	return __builtin_bswap32(i);
}

void print_word(const char* label, uint32_t i) {
	printf("WORD %s: 0b" BYTE_TO_BINARY_PATTERN BYTE_TO_BINARY_PATTERN BYTE_TO_BINARY_PATTERN BYTE_TO_BINARY_PATTERN"\n", label, BYTE_TO_BINARY(i >> 24), BYTE_TO_BINARY(i >> 16), BYTE_TO_BINARY(i >> 8), BYTE_TO_BINARY(i));
}
