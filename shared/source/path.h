#pragma once
#include <stddef.h>

struct path
{
    size_t component_count;
    char **components;
    int has_protocol;
};

void path_dispose(struct path *path);
void path_from_string(struct path *path, char *string);
char *path_to_string(struct path *path, int is_dir);
void path_insert_component(struct path *path, size_t index, char *component);
void path_remove_component(struct path *path, size_t index);
char *path_get_extension(struct path *path);
char *path_get_file_stem(struct path *path);
