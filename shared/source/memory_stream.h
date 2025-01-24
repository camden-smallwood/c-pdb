#pragma once
#include <stdint.h>
#include <stdio.h>

enum memory_stream_flags
{
    MEMORY_STREAM_IS_WRITEABLE = 1 << 0,
    MEMORY_STREAM_IS_RESIZABLE = 1 << 1,
};

struct memory_stream
{
    void *address;
    size_t size;
    size_t position;
    uint8_t flags;
};

void memory_stream_seek(struct memory_stream *stream, long offset, int whence);
void memory_stream_read(void *destination, size_t size, size_t count, struct memory_stream *stream);
void memory_stream_write(void *source, size_t size, size_t count, struct memory_stream *stream);
void memory_stream_resize(struct memory_stream *stream, size_t size);
