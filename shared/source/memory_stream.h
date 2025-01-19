#pragma once
#include <stdint.h>
#include <stdio.h>

struct memory_stream
{
    void *address;
    size_t size;
    size_t position;
};

void memory_stream_seek(struct memory_stream *stream, long offset, int whence);
void memory_stream_read(void *destination, size_t size, size_t count, struct memory_stream *stream);
