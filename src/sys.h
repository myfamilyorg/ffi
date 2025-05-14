#ifndef __SYS_H_
#define __SYS_H_

#include <types.h>

#define OPEN_RDONLY 0x1
#define OPEN_WRONLY (0x1 << 1)
#define OPEN_RDWR (OPEN_RDONLY | OPEN_WRONLY)
#define OPEN_CREATE (0x1 << 2)

#define PAGE_SIZE (getpagesize())

#define CHECK_AND_SET_ERRORS()          \
	if (errno == EFAULT)            \
		err = ERR_EFAULT;       \
	else if (errno == EIO)          \
		err = ERR_EIO;          \
	else if (errno == ENOSYS)       \
		err = ERR_ENOSYS;       \
	else if (errno == EACCES)       \
		err = ERR_EACCES;       \
	else if (errno == EAGAIN)       \
		err = ERR_EAGAIN;       \
	else if (errno == EBADF)        \
		err = ERR_EBADF;        \
	else if (errno == EWOULDBLOCK)  \
		err = ERR_EWOULDBLOCK;  \
	else if (errno == EINVAL)       \
		err = ERR_EINVAL;       \
	else if (errno == EROFS)        \
		err = ERR_EROFS;        \
	else if (errno == ENODEV)       \
		err = ERR_ENODEV;       \
	else if (errno == ENOMEM)       \
		err = ERR_ENOMEM;       \
	else if (errno == EOVERFLOW)    \
		err = ERR_EOVERFLOW;    \
	else if (errno == EPERM)        \
		err = ERR_EPERM;        \
	else if (errno == ETXTBSY)      \
		err = ERR_ETXTBSY;      \
	else if (errno == EBUSY)        \
		err = ERR_EBUSY;        \
	else if (errno == EDQUOT)       \
		err = ERR_EDQUOT;       \
	else if (errno == EEXIST)       \
		err = ERR_EEXIST;       \
	else if (errno == EFBIG)        \
		err = ERR_EFBIG;        \
	else if (errno == EINTR)        \
		err = ERR_EINTR;        \
	else if (errno == EISDIR)       \
		err = ERR_EISDIR;       \
	else if (errno == ELOOP)        \
		err = ERR_ELOOP;        \
	else if (errno == EMFILE)       \
		err = ERR_EMFILE;       \
	else if (errno == ENAMETOOLONG) \
		err = ERR_ENAMETOOLONG; \
	else if (errno == ENFILE)       \
		err = ERR_ENFILE;       \
	else if (errno == EOPNOTSUPP)   \
		err = ERR_EOPNOTSUPP;   \
	else if (errno == ENOENT)       \
		err = ERR_ENOENT;       \
	else if (errno == ENOSPC)       \
		err = ERR_ENOSPC;       \
	else if (errno == ENOTDIR)      \
		err = ERR_ENOTDIR;      \
	else if (errno == ENXIO)        \
		err = ERR_ENXIO;        \
	else                            \
		err == ERR_UNKNOWN;

int getpagesize();
uint64_t time(uint64_t *second);
uint32_t sleep(uint32_t seconds);
int rand_bytes(void *buf, size_t length);
void *map(size_t pages);
void *fmap(int fd, size_t offset, size_t pages);
const void *fview(int fd, size_t offset, size_t pages);
int unmap(void *addr, size_t pages);
int mapsync(void *addr, size_t pages, bool async);
int openfd(const char *pathname, int flags);
int closefd(int fd);
int64_t fsize(int fd);
int fresize(int fildes, size_t length);

#endif	// __SYS_H_
