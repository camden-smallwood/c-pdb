#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "memory_stream.h"

void memory_stream_seek(struct memory_stream *stream, long offset, int whence)
{
    assert(stream);

    switch (whence)
    {
    case SEEK_SET:
        assert(offset >= 0 && (size_t)offset <= stream->size);
        stream->position = (size_t)offset;
        break;
    
    case SEEK_CUR:
        if (offset < 0)
        {
            size_t inverse_offset = (size_t)-offset;
            assert(inverse_offset <= stream->position);
            stream->position -= inverse_offset;
        }
        else if (offset > 0)
        {
            size_t position = (size_t)offset;
            assert(stream->position + position <= stream->size);
            stream->position += position;
        }
        break;
    
    case SEEK_END:
        assert(offset >= 0 && (size_t)offset <= stream->size);
        stream->position = stream->size - (size_t)offset;
        break;
    
    default:
        fprintf(stderr, "%s:%i: ERROR: Unhandled whence value: %i\n", __FILE__, __LINE__, whence);
        exit(EXIT_FAILURE);
    }
}

void memory_stream_read(void *destination, size_t size, size_t count, struct memory_stream *stream)
{
    assert(destination);
    assert(stream);

    if (!size || !count)
        return;

    size_t read_size = size * count;
    assert(stream->position + read_size < stream->size);

    memcpy(destination, (char *)stream->address + stream->position, read_size);
    stream->position += read_size;
}

void memory_stream_write(void *source, size_t size, size_t count, struct memory_stream *stream)
{
    assert(stream);
    assert(stream->flags & MEMORY_STREAM_IS_WRITEABLE);

    if (!size || !count)
        return;

    size_t write_size = size * count;
    assert(stream->position + write_size <= stream->size);

    memcpy((char *)stream->address + stream->position, source, write_size);
    stream->position += write_size;
}

void memory_stream_resize(struct memory_stream *stream, size_t size)
{
    assert(stream);
    assert(stream->flags & MEMORY_STREAM_IS_RESIZABLE);

    stream->address = realloc(stream->address, size);
    assert(stream->address);

    stream->size = size;

    if (stream->position > stream->size)
        stream->position = stream->size;
}
