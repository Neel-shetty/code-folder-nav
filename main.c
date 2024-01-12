// #include <cstdlib>
#include <dirent.h>
#include <linux/limits.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <wchar.h>

#define TYPE_FOLDER 4
#define TYPE_FILE 8

typedef struct num_map {
  char name[256];
  int type;
} num_map;

num_map folders[1000];

void logic(DIR *d, char *wd, char *prev_dir, char *command) {
  char *option = malloc(256);
  strcpy(option, "-1");

  // file menu loop
  while (atoi(option) != 0) {
    d = opendir(wd);

    if (!d) {
      // open file
      asprintf(&command, "xdg-open %s 1>/dev/null 2>&1", wd);
      system(command);
      system("exit");
      break;
    }

    // print menu
    struct dirent *entries;
    int i = 1;
    while ((entries = readdir(d)) != NULL) {
      // early exit for the current dir and prev dir options
      if (strcmp(entries->d_name, ".") == 0 ||
          strcmp(entries->d_name, "..") == 0) {
        continue;
      }
      if (entries->d_type == TYPE_FOLDER) {
        wprintf(L"dir   %d: %s\n", i, entries->d_name);
      } else {
        wprintf(L"file  %d: %s\n", i, entries->d_name);
      }
      // store folder info in folders array based on index
      strcpy(folders[i].name, entries->d_name);
      folders[i].type = entries->d_type;
      i++;
    }

    // take input from user
    printf("select the number of the folder you want to go to\n");
    scanf("%s", option);

    // concat string (path + folder name)
    char *cd;
    asprintf(&cd, "%s/%s", wd, folders[atoi(option)].name);
    printf("current dir -- %s\n", cd);

    // options to open terminal and file
    if (*option == 't') {
      // open dir in terminal
      asprintf(&command, "kitty --detach %s", wd);
      system(command);
      system("exit");
      break;
    } else if (*option == 'o') {
      // open file
      asprintf(&command, "xdg-open %s 1>/dev/null 2>&1", wd);
      system(command);
      system("exit");
      break;
    }

    // prev dir for fallback incase the option selected is a file
    prev_dir = wd;
    wd = cd;
    closedir(d);
  }

  // when quit
  if (atoi(option) == 0) {
    char *command;
    char *cd_into_dir;

    // concact strings (command + path)
    // asprintf(&command, "kitty --detach nvim %s", wd);
    asprintf(&cd_into_dir, "cd %s", wd);
    asprintf(&command, "nvim %s", wd);
    system(cd_into_dir);
    system(command);
    system("exit");
  }
  return;
}

int main(int argc, char *argv[]) {

  // init variables
  DIR *d;
  char *command;
  if (argc == 2) {
    char *wd = argv[1];
    char *prev_dir = wd;
    logic(d, wd, prev_dir, command);
  } else {
    char *wd = "/home/neel/code";
    char *prev_dir = wd;
    logic(d, wd, prev_dir, command);
  }

  // option management
  //
  return 0;
}
