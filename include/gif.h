#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

/* Function prototypes for creating a GIF from PPM files */
void list_ppm_files(const char *directory, char ***ppm_files, int *file_count);
void create_gif(const char *output_gif, char **ppm_files, int file_count, int delay);