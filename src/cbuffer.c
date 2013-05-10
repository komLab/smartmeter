#include "cbuffer.h"

int alloc_buffer(cbuffer *buffer, int size)
{
	buffer->addr = malloc(size * sizeof(reading));
	if(buffer->addr == NULL) {
		printf("Can't allocate virtual memory.\n");
		return 1;
	}

	buffer->size = size + 1;
	buffer->head = 0;
	buffer->tail = 0;

	//printf("allocate virtual memory. CHECK!\n");
	return 0;
}

int free_buffer(cbuffer *buffer)
{
	if (buffer->addr == NULL) {
		printf("Can't free virtual memory.\n");
		return 1;
	}

	free(buffer->addr);
	//printf("free virtual memory. CHECK!\n");

	return 0;
}

void add_buffer(cbuffer *buffer, reading *element)
{
	buffer->addr[buffer->tail] = *element;
	buffer->tail = (buffer->tail + 1) % buffer->size;
	if (buffer->tail == buffer->head)
		buffer->head = (buffer->head + 1) % buffer->size;
}

void pop_buffer(cbuffer *buffer, reading *element)
{
	*element = buffer->addr[buffer->head];
	buffer->head = (buffer->head + 1) % buffer->size;
}

int buffer_is_full(cbuffer *buffer)
{
	return (buffer->tail + 1) % buffer->size == buffer->head;
}

int buffer_is_empty(cbuffer *buffer)
{
	return buffer->tail == buffer->head;
}
