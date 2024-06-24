
#include "../includes/buda.h"


namespace BUDA
{	


char *get_filenames_string(const char *dir_path) 
{
  DIR *dir; struct dirent *entry;

  if ((dir = opendir(dir_path)) == NULL) { log("open dir error: %s", dir_path); return NULL; }

  char *filenames_str = NULL; size_t allocated_size = 0, written_chars = 0;

  while ((entry = readdir(dir)) != NULL) {
      // Skip non-regular files (directories, symbolic links, etc.)
      if (entry->d_type != DT_REG) {
          continue;
      }

      // Allocate or expand the filenames_str buffer if needed
      size_t entry_len = strlen(entry->d_name);
      if (written_chars + entry_len + 2 >= allocated_size) { // +1 for space, +1 for null terminator
          allocated_size = allocated_size == 0 ? entry_len + 16 : allocated_size * 2;
          filenames_str = realloc(filenames_str, allocated_size);
          if (filenames_str == NULL) {
              perror("realloc");
              closedir(dir);
              return NULL;
          }
      }

      // Append filename with space
      memcpy(filenames_str + written_chars, entry->d_name, entry_len);
      written_chars += entry_len;
      filenames_str[written_chars++] = ' ';
  }

  // Add null terminator if not empty
  if (written_chars > 0) {
      filenames_str[written_chars - 1] = '\0'; // Remove trailing space
  } else {
      // Free buffer if no filenames were found
      free(filenames_str);
      filenames_str = NULL;
  }

  closedir(dir);
  return filenames_str;
}


}