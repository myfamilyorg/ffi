#include <errno.h>
#include <fcntl.h>
#include <sys.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <types.h>

void *map(size_t pages) {
	if (pages == 0) return NULL;

	size_t size = pages * PAGE_SIZE;
	if (size / PAGE_SIZE != pages) {
		errno = ENOMEM;
		return NULL;
	}

	void *ret = mmap(NULL, pages * PAGE_SIZE, PROT_READ | PROT_WRITE,
			 MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (ret == MAP_FAILED) return NULL;
	return ret;
}

int mapsync(void *addr, size_t pages, bool async) {
	if (addr == NULL || pages == 0) {
		errno = EINVAL;
		return -1;
	}

	size_t page_size = getpagesize();
	size_t size = pages * page_size;
	if (size / page_size != pages) {
		errno = EINVAL;
		return -1;
	}

	int flags = async ? MS_ASYNC : MS_SYNC;
	return msync(addr, size, flags);
}

int openfd(const char *path, int flags) {
	return open(path, flags, S_IRUSR | S_IWUSR);
}

size_t fsize(int fd) {
	struct stat buf;
	if (fstat(fd, &buf)) return -1;
	return buf.st_size;
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
		errno = EINVAL;
		return NULL;
	}

	if (offset % page_size != 0) {
		errno = EINVAL;
		return NULL;
	}

	if (fd < 0) {
		errno = EBADF;
		return NULL;
	}

	void *ret;
	if (write)
		ret = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd,
			   offset);
	else
		ret = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, offset);
	if (ret == MAP_FAILED) {
		return NULL;
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
	return munmap(addr, size);
}

int sys(int argc, char **argv) { return 0; }
