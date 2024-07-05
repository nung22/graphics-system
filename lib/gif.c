#include "gif.h"

// Comparison function for sorting filenames
static int compare(const void *a, const void *b) {
  return strcmp(*(const char **)a, *(const char **)b);
}

// Function to list all PPM files in a directory
void list_ppm_files(const char *directory, char ***ppm_files, int *file_count)
{
  struct dirent *entry;
  DIR *dp = opendir(directory);
  if (!dp)
  {
    perror("Error opening directory");
    exit(EXIT_FAILURE);
  }

  *file_count = 0;
  int capacity = 10;
  *ppm_files = malloc(capacity * sizeof(char *));
  if (!*ppm_files)
  {
    perror("Memory allocation failed");
    closedir(dp);
    exit(EXIT_FAILURE);
  }

  while ((entry = readdir(dp)))
  {
    if (strstr(entry->d_name, ".ppm"))
    {
      if (*file_count >= capacity)
      {
        capacity *= 2;
        *ppm_files = realloc(*ppm_files, capacity * sizeof(char *));
        if (!*ppm_files)
        {
          perror("Memory reallocation failed");
          closedir(dp);
          exit(EXIT_FAILURE);
        }
      }
      char path[1024];
      snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);
      (*ppm_files)[*file_count] = strdup(path);
      if (!(*ppm_files)[*file_count])
      {
        perror("Memory allocation failed");
        closedir(dp);
        exit(EXIT_FAILURE);
      }
      (*file_count)++;
    }
  }

  closedir(dp);

  // Sort the PPM files alphabetically
    qsort(*ppm_files, *file_count, sizeof(char *), compare);
}

// Function to create a GIF from PPM files
void create_gif(const char *output_gif, char **ppm_files, int file_count, int delay)
{
  char command[4096] = "magick -delay ";
  char delay_str[10];
  snprintf(delay_str, sizeof(delay_str), "%d", delay);
  strcat(command, delay_str);
  strcat(command, " ");

  for (int i = 0; i < file_count; i++)
  {
    strcat(command, ppm_files[i]);
    strcat(command, " ");
  }

  strcat(command, output_gif);
  system(command);
}
