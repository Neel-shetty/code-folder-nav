// #include <cstdlib>
#include <dirent.h>
#include <linux/limits.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

typedef struct num_map {
  char name[256];
  int type;
} num_map;

num_map folders[1000];

int main() {
  DIR *d;
  char *wd = "/home/neel/code";
  char *prev_dir = wd;
  int option = -1;
  setlocale(LC_CTYPE, "");
  wchar_t folder_icon = 0x2606;
  while (option != 0) {
    d = opendir(wd);
    if (!d) {
      printf("unable to read dir\n");
      wd = prev_dir;
      option = -1;
      continue;
    }
    struct dirent *entries;
    int i = 1;
    while ((entries = readdir(d)) != NULL) {
      if (strcmp(entries->d_name, ".") == 0 ||
          strcmp(entries->d_name, "..") == 0) {
        continue;
      }
      if (entries->d_type == 4) {
        wprintf(L"dir   %d: %s\n", i, entries->d_name);
        // printf("%d: %s\n", i, entries->d_name);
      } else {
        wprintf(L"file  %d: %s\n", i, entries->d_name);
      }
      strcpy(folders[i].name, entries->d_name);
      folders[i].type = entries->d_type;
      i++;
    }

    printf("select the number of the folder you want to go to\n");
    scanf("%d", &option);
    char *cd;
    asprintf(&cd, "%s/%s", wd, folders[option].name);
    printf("%s\n", cd);
    prev_dir = wd;
    wd = cd;
    closedir(d);
  }
  if (option == 0) {
    char *command;
    asprintf(&command, "kitty --detach nvim %s", wd);
    system(command);
  }
  return 0;
}
