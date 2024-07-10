#include <stdio.h>
#include "gif.h"

// Covert PPM files to GIF
int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Usage: create-gif <directory> <output.gif> <delay>\n");
        return EXIT_FAILURE;
    }

    const char *directory = argv[1];
    const char *gif_path = argv[2];
    int delay = atoi(argv[3]);

    char **ppm_files;
    int file_count;
    list_ppm_files(directory, &ppm_files, &file_count);

    if (file_count > 0) {
        create_gif(gif_path, ppm_files, file_count, delay);
    } else {
        fprintf(stderr, "No PPM files found in the directory.\n");
    }

    for (int i = 0; i < file_count; i++) {
        free(ppm_files[i]);
    }
    free(ppm_files);

    return EXIT_SUCCESS;
}