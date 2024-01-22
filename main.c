// #include <cstdlib>
#include <dirent.h>
#include <linux/limits.h>
#include <locale.h>
#include <ncurses.h>
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

void print_menu(char *wd) {
  struct dirent **entries;
  int n = scandir(wd, &entries, NULL, alphasort);

  for (int j = 1; entries[j] != NULL; j++) {
    printw("%d: %s\n", j, entries[j]->d_name);
    strcpy(folders[j].name, entries[j]->d_name);
  }
  refresh();
}

void logic(DIR *d, char *wd, char *prev_dir, char *command) {
  char *option = malloc(256);
  strcpy(option, "-1");

  // file menu loop
  while (strcmp(option, "0") != 0) {
    if (strcmp(option, "q") == 0)
      break;
    d = opendir(wd);

    if (!d) {
      // open file
      asprintf(&command, "xdg-open %s 1>/dev/null 2>&1", wd);
      system(command);
      system("exit");
      break;
    }

    // print menu
    print_menu(wd);

    // take input from user
    printw("select the number of the folder you want to go to (0 to open nvim "
           "here)\n");
    int PATH_LENGTH = 255;
    getnstr(option, PATH_LENGTH);
    clear();

    char *cd;

    asprintf(&cd, "%s/%s", wd, folders[atoi(option)].name);
    printw("current dir -- %s\n", cd);

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
  if (strcmp(option, "0") == 0) {
    char *command;
    char *cd_into_dir;
    // concact strings (command + path)
    asprintf(&cd_into_dir, "cd %s", wd);
    asprintf(&command, "nvim %s", wd);
    system(cd_into_dir);
    system(command);
    system("exit");
  }
  clear();
  return;
}

int main(int argc, char *argv[]) {

  // init variables
  initscr(); /* Start curses mode 		  */
  raw();
  keypad(stdscr, TRUE);
  DIR *d;
  char *command;
  if (argc == 2) {
    // TODO: move wd up and dont call logic func twice
    char *wd = argv[1];
    char *prev_dir = wd;
    logic(d, wd, prev_dir, command);
  } else {
    char *wd = "/home/neel/code";
    char *prev_dir = wd;
    logic(d, wd, prev_dir, command);
  }

  endwin(); /* End curses mode		  */
  return 0;
}
