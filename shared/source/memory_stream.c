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
        else
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
    assert(stream);
    assert(destination);

    size_t read_size = size * count;

    assert(stream->position + read_size < stream->size);

    if (!size || !count)
        return;
    
    memcpy(destination, (char *)stream->address + stream->position, read_size);
    stream->position += read_size;
}
