#include <types.h>

// for timespec/CLOCK_REALTIME structures
#include <time.h>
void *malloc(size_t);
void *realloc(void *ptr, size_t);
void free(void *);
int getentropy(void *buf, size_t length);

void *alloc(size_t size) {
	void *ptr = malloc(size);
	return ptr;
}

void release(void *ptr) { free(ptr); }

void *resize(void *ptr, size_t len) {
	void *ret = realloc(ptr, len);
	return ret;
}

uint64_t getmicros() {
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	return (uint64_t)((__int128_t)now.tv_sec * 1000000) +
	       (uint64_t)(now.tv_nsec / 1000);
}

int sleep_millis(uint64_t millis) {
	struct timespec ts;
	ts.tv_sec = millis / 1000;
	ts.tv_nsec = (millis % 1000) * 1000000;
	int ret = nanosleep(&ts, 0);
	return ret;
}

int rand_bytes(byte *buf, size_t length) { return getentropy(buf, length); }

