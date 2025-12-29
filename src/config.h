#ifndef __CONFIG_H__
#define __CONFIG_H__

#define KERNEL_CODE_SELECTOR        0x08
#define KERNEL_DATA_SELECTOR        0x10

#define MYOS_TOTAL_INTERRUPTS       512

// 100 MB heap size
#define MYOS_HEAP_SIZE_BYTES        104857600
#define MYOS_HEAP_BLOCK_SIZE        4096
#define MYOS_HEAP_ADDRESS           0x01000000
#define MYOS_HEAP_TABLE_ADDRESS     0x00007E00

// Filesystem
#define MYOS_MAX_PATH               100
#define MYOS_MAX_FILESYSTEMS        12
#define MYOS_MAX_FILE_DESCRIPTORS   512

#endif
