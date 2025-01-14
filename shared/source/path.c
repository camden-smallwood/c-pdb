#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "path.h"
#include "utils.h"

void path_dispose(struct path *path)
{
    assert(path);

    for (size_t i = 0; i < path->component_count; i++)
        free(path->components[i]);
    
    free(path->components);
}

static char* next_path_token(char* string, char** next_token, int skip_token, int* has_protocol)
{
    if (next_token)
        *next_token = NULL;
    if (has_protocol)
        *has_protocol = 0;

    char* current_protocol = strchr(string, ':');
    char* current_forward = strchr(string, '/');
    char* current_back = strchr(string, '\\');

    char* token = NULL;
    if (current_protocol)
    {
        token = string;

        if (next_token)
        {
            char* after_protocol = current_protocol + 1;
            *next_token = next_path_token(after_protocol, NULL, 0, NULL);
        }
        if (has_protocol)
            *has_protocol = 1;
    }
    else
    {
        if (current_forward && current_back)
            token = __min(current_forward, current_back);
        else
            token = current_forward ? current_forward : current_back;
        if (token)
        {
            if (next_token)
            {
                char* after_token = token + 1;
                *next_token = next_path_token(after_token, NULL, 0, NULL);
            }
            if (skip_token)
                token++;
        }
    }
   
    return token;
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
        char* next;
        int has_protocol;
        current = next_path_token(current, &next, 1, &has_protocol);
        if (has_protocol)
            path->has_protocol = 1;
        if (!current)
            break;

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

            if (path->component_count)
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
        if(!path->has_protocol || i > 0)
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

char *path_get_extension(struct path *path)
{
    assert(path);

    if (path->component_count == 0)
        return NULL;
    
    char *ext = strrchr(path->components[path->component_count - 1], '.');

    if (!ext)
        return NULL;
    
    return strdup(ext + 1);
}

char *path_get_file_stem(struct path *path)
{
    assert(path);

    if (path->component_count == 0)
        return NULL;
    
    char *component = path->components[path->component_count - 1];
    char *ext = strrchr(component, '.');

    if (!ext)
        return strdup(component);
    
    size_t length = (size_t)(ext - component);

    if (length == 0)
        return strdup(component);
    
    char *result = malloc(length + 1);
    assert(result);
    
    memcpy(result, component, length);
    result[length] = '\0';

    return result;
}
