#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "path.h"
#include "utils.h"

void path_dispose(struct path *path)
{
    assert(path);

    for (size_t i = 0; i < path->component_count; i++)
        free(path->components[i]);
    
    free(path->components);
}

void path_from_string(struct path *path, char *string)
{
    assert(path);
    
    memset(path, 0, sizeof(*path));

    if (!string)
        return;

    size_t total_length = strlen(string);

    char *current = string;

    while (current)
    {
        current = strchr(current, '/');
        if (!current)
            break;
        current++;

        char *next = strchr(current, '/');
        size_t length = next ? (next - current) : (total_length - (current - string));

        char *component = calloc(length + 1, sizeof(char));
        assert(component);

        strncpy(component, current, length);

        if (strcmp(component, ".") == 0)
        {
            free(component);
        }
        else if (strcmp(component, "..") == 0)
        {
            free(component);
            DYNARRAY_POP(path->components, path->component_count, sizeof(component));
        }
        else
        {
            DYNARRAY_PUSH(path->components, path->component_count, component);
        }

        current = next;
    }
}

char *path_to_string(struct path *path, int is_dir)
{
    assert(path);

    char *result = strdup("");
    assert(result);

    for (size_t i = 0; i < path->component_count; i++)
    {
        string_append(&result, "/");
        string_append(&result, path->components[i]);
    }

    if (is_dir)
        string_append(&result, "/");

    return result;
}

void path_insert_component(struct path *path, size_t index, char *component)
{
    assert(path);
    assert(index <= path->component_count);
    assert(component);

    size_t move_count = path->component_count - index;

    path->component_count++;
    path->components = realloc(path->components, path->component_count * sizeof(*path->components));
    assert(path->components);

    if (move_count)
        memmove(&path->components[index + 1], &path->components[index], move_count * sizeof(*path->components));
    
    path->components[index] = strdup(component);
    assert(path->components[index]);
}

void path_remove_component(struct path *path, size_t index)
{
    assert(path);
    assert(index < path->component_count);

    path->component_count--;

    size_t move_count = path->component_count - index;

    if (move_count)
        memmove(&path->components[index], &path->components[index + 1], move_count * sizeof(*path->components));
    
    path->components = realloc(path->components, path->component_count * sizeof(*path->components));
    assert(path->components);
}
