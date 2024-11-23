#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pdb.h"
#include "utils.h"

int main(int argc, const char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: dumper <pdb-file>\n");
        exit(EXIT_FAILURE);
    }

    const char *pdb_path = argv[1];
    FILE *pdb_file = fopen(pdb_path, "r");

    if (!pdb_file)
    {
        fprintf(stderr, "ERROR: Failed to open \"%s\"\n", pdb_path);
        exit(EXIT_FAILURE);
    }

    struct pdb_data pdb_data;
    pdb_data_read(&pdb_data, pdb_file);

    fclose(pdb_file);

    pdb_data_print(&pdb_data, 0, stdout);
    pdb_data_dispose(&pdb_data);

    exit(EXIT_SUCCESS);
}
