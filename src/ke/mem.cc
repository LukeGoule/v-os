#include <mem.h>

int memcmp(const void* aptr, const void* bptr, size_t size) {
	const unsigned char* a = (const unsigned char*) aptr;
	const unsigned char* b = (const unsigned char*) bptr;
	for (size_t i = 0; i < size; i++) {
		if (a[i] < b[i])
			return -1;
		else if (b[i] < a[i])
			return 1;
	}
	return 0;
}


extern void* end_sym;
uint32_t offset = 0;

void* malloc(size_t size) {
	if (size == 0) {
		return NULL;
	}

	
	void* ret = (void*)(&end_sym + offset);

	offset += 512;

	return ret;

}

uint32_t* GetNextAddr() {
	return (uint32_t*)end_sym + offset;
}