#ifndef __SYS_H_
#define __SYS_H_

#include <types.h>

#define OPEN_RDONLY 0x1
#define OPEN_WRONLY (0x1 << 1)
#define OPEN_RDWR (OPEN_RDONLY | OPEN_WRONLY)
#define OPEN_CREATE (0x1 << 2)

#define PAGE_SIZE (getpagesize())

int getpagesize();
uint64_t time(uint64_t *second);
uint32_t sleep(uint32_t seconds);
int rand_bytes(void *buf, size_t length);
void *map(size_t pages);
void *fmap(int fd, size_t offset, size_t pages);
void *fview(int fd, size_t offset, size_t pages);
int unmap(void *addr, size_t pages);
int mapsync(void *addr, size_t pages, bool async);
int openfd(const char *pathname, int flags);
int closefd(int fd);
int64_t fsize(int fd);
int fresize(int fildes, size_t length);

#endif	// __SYS_H_
