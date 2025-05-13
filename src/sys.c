#include <errno.h>
#include <error.h>
#include <signal.h>
#include <sys.h>
#include <sys/fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <types.h>

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

int getentropy(void *buf, size_t length);
int ftruncate(int fd, size_t length);
int close(int fd);

int rand_bytes(void *buf, size_t length) {
	if (getentropy(buf, length)) {
		CHECK_AND_SET_ERRORS()
		return err;
	}
	return 0;
}

void *map(size_t pages) {
	if (pages == 0) return NULL;

	size_t size = pages * PAGE_SIZE;
	if (size / PAGE_SIZE != pages) {
		err = ERR_ENOMEM;
		return NULL;
	}

	void *ret = mmap(NULL, pages * PAGE_SIZE, PROT_READ | PROT_WRITE,
			 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (ret == MAP_FAILED) {
		CHECK_AND_SET_ERRORS();
		return NULL;
	} else if (ret == NULL) {
		CHECK_AND_SET_ERRORS();
	}
	return ret;
}

int mapsync(void *addr, size_t pages, bool async) {
	if (addr == NULL || pages == 0) {
		err = ERR_EINVAL;
		return err;
	}

	size_t page_size = getpagesize();
	size_t size = pages * page_size;
	if (size / page_size != pages) {
		err = ERR_EINVAL;
		return err;
	}

	int flags = async ? MS_ASYNC : MS_SYNC;
	if (msync(addr, size, flags)) {
		CHECK_AND_SET_ERRORS()
		return err;
	}
	return 0;
}

int openfd(const char *path, int flags) {
	int cflags = 0;
	if ((flags & OPEN_RDWR) == OPEN_RDWR)
		cflags = O_RDWR;
	else if ((flags & OPEN_RDONLY) == OPEN_RDONLY)
		cflags = O_RDONLY;
	else if ((flags & OPEN_WRONLY) == OPEN_WRONLY)
		cflags = O_WRONLY;
	if (flags & OPEN_CREATE) cflags |= O_CREAT;

	int fd = open(path, cflags, S_IRUSR | S_IWUSR);
	if (fd < 0) {
		CHECK_AND_SET_ERRORS()
		return err;
	}
	return fd;
}

int closefd(int fd) {
	if (close(fd)) {
		CHECK_AND_SET_ERRORS()
		return err;
	}
	return 0;
}

int64_t fsize(int fd) {
	struct stat buf;
	if (fstat(fd, &buf)) {
		CHECK_AND_SET_ERRORS()
		return err;
	}
	return buf.st_size;
}

int fresize(int fd, size_t length) {
	if (ftruncate(fd, length)) {
		CHECK_AND_SET_ERRORS()
		return err;
	}
	return 0;
}

void *fimpl(int fd, size_t offset, size_t pages, bool write) {
	if (pages == 0) {
		return NULL;
	}

	long page_size = PAGE_SIZE;
	if (page_size == -1) {
		return NULL;
	}

	size_t size = pages * page_size;
	if (size / page_size != pages) {
		err = ERR_EINVAL;
		return NULL;
	}

	if (offset % page_size != 0) {
		err = ERR_EINVAL;
		return NULL;
	}

	if (fd < 0) {
		err = ERR_EBADF;
		return NULL;
	}

	void *ret;
	if (write)
		ret = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd,
			   offset);
	else
		ret = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, offset);

	if (ret == MAP_FAILED) {
		CHECK_AND_SET_ERRORS();
		return NULL;
	} else if (ret == NULL) {
		CHECK_AND_SET_ERRORS();
	}
	return ret;
}

void *fmap(int fd, size_t offset, size_t pages) {
	return fimpl(fd, offset, pages, true);
}

void *fview(int fd, size_t offset, size_t pages) {
	return fimpl(fd, offset, pages, false);
}

int unmap(void *addr, size_t pages) {
	if (addr == NULL) {
		errno = EINVAL;
		return -1;
	}
	if (pages == 0) return 0;

	size_t size = pages * PAGE_SIZE;
	if (size / PAGE_SIZE != pages) {
		errno = EINVAL;
		return -1;
	}
	if (munmap(addr, size)) {
		CHECK_AND_SET_ERRORS();
		return err;
	}
	return 0;
}

int sys(int argc, char **argv) { return 0; }
