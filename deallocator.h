#ifndef DEALLOCATOR_H
#define DEALLOCATOR_H

/*
 * # Usage:
 *
 * DA_STRUCT(100) da;
 * deallocator_init(&da.head, 100);
 * printf("%s\n", da_c(&da, strdup("some string")));
 * char *string1 = da_c(&da, (char *) calloc(1, sizeof(char)));
 * deallocator_cleanup(&da.head);
 */

#include <stddef.h>

struct deallocator
{
	size_t capacity;
	size_t size;
	void *pointers[];
};

void deallocator_init(struct deallocator *deallocator, size_t size);
void deallocator_cleanup(struct deallocator *deallocator);
char *da_c(void *deallocator, char *ptr);

#define DA_STRUCT(size) \
	struct deallocator_ ## size \
	{ \
		struct deallocator head; \
		void *pointers[size]; \
	}

#define DA_X(deallocator, type, ptr) ((type) da_c(deallocator, ptr))

#endif
