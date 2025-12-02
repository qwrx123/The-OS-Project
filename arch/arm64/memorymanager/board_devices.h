#ifndef BOARD_DEVICES_H
#define BOARD_DEVICES_H

#include "kernel/types.h"
#include "kernel/stddef.h"

#define UART0_BASE_PHYS 0x09000000ULL
#define UART0_SIZE 0x1000ULL

typedef struct
{
	const char *name;
	phys_addr_t base_addr;
	size_t size;
	uint8_t mem_type;
	uint64_t access_perm;
	uint64_t shareability;
	uint64_t pxn;
	uint64_t uxn;
} device_config_t;

extern const device_config_t early_device_map[];
extern const size_t early_device_map_count;

#endif