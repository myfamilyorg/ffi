#ifndef _TYPES_H__
#define _TYPES_H__

int printf(const char *, ...);
void exit(int);

#define NULL ((void *)0)

typedef unsigned long size_t;
typedef __int128_t int128_t;
typedef __uint128_t uint128_t;
typedef unsigned long long uint64_t;
typedef long long int64_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef short unsigned int uint16_t;
typedef short int int16_t;
typedef signed char int8_t;
typedef unsigned char uint8_t;
typedef unsigned char byte;
typedef _Bool bool;
#define true ((_Bool)1)
#define false ((_Bool)0)

#define CheckType(t, exp)                                               \
	if (sizeof(t) != exp) {                                         \
		printf("expected sizeof(%s) == %d, Found %lu", #t, exp, \
		       sizeof(t));                                      \
		exit(-1);                                               \
	}

static __attribute__((constructor)) void check_sizes() {
	uint16_t test = 0x1234;
	uint8_t *bytes = (uint8_t *)&test;
	if (bytes[0] != 0x34) {	 // Big-endian if first byte is 0x12
		printf("Error: Big-endian system not supported\n");
		exit(-1);
	}
	CheckType(uint8_t, 1);
	CheckType(int8_t, 1);
	CheckType(uint16_t, 2);
	CheckType(int16_t, 2);
	CheckType(uint32_t, 4);
	CheckType(int32_t, 4);
	CheckType(uint64_t, 8);
	CheckType(int64_t, 8);
	CheckType(uint128_t, 16);
	CheckType(int128_t, 16);
	CheckType(byte, 1);
	CheckType(size_t, 8);
	if (!true) {
		printf("expected true to be true\n");
		exit(-1);
	}
	if (false) {
		printf("expected false to be false\n");
		exit(-1);
	}
	bool b = true;
	if (!true) {
		printf("expected true to be true\n");
		exit(-1);
	}
	if (NULL) {
		printf("expected NULL to be NULL\n");
		exit(-1);
	}
}

#endif	// _TYPES_H__
