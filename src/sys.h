#ifndef __SYS_H_
#define __SYS_H_

#include <types.h>

#define PAGE_SIZE (getpagesize())

int getpagesize();
uint64_t time(uint64_t *second);
uint32_t sleep(uint32_t seconds);
int getentropy(void *buf, size_t length);
void *map(size_t pages);
void *fmap(int fd, size_t offset, size_t pages);
void *fview(int fd, size_t offset, size_t pages);
int unmap(void *addr, size_t pages);
int mapsync(void *addr, size_t pages, bool async);
int ftruncate(int fildes, size_t length);
int openfd(const char *pathname, int flags);
int close(int fd);
size_t fsize(int fd);
int ftruncate(int fildes, size_t length);

#endif	// __SYS_H_
