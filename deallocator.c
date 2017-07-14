#include "deallocator.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

void deallocator_init(struct deallocator *deallocator, size_t size)
{
	assert(deallocator);
	deallocator->capacity = size;
	deallocator->size = 0;
}

void deallocator_cleanup(struct deallocator *deallocator)
{
	assert(deallocator);
	for (size_t i = 0; i < deallocator->size; ++i)
		free(deallocator->pointers[i]);
	deallocator->size = 0;
}

char *da_c(void *deallocator, char *ptr)
{
	struct deallocator *da = (struct deallocator *) deallocator;
	assert(da->size < da->capacity);
	da->pointers[da->size++] = ptr;
	return ptr;
}

