#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

// power of 2
// defualt 16384

#define BUFFER_SIZE 16384
typedef struct {
	uint32_t value[2];
}reading;

typedef struct {
	int size;
	int head;
	int tail;
	reading *addr;
}cbuffer;

int alloc_buffer(cbuffer*, int);
int free_buffer(cbuffer*);
void add_buffer(cbuffer*, reading*);
void pop_buffer(cbuffer*, reading*);
int buffer_is_full(cbuffer*);
int buffer_is_empty(cbuffer*);
