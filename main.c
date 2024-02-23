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
#define STRLEN 255

typedef struct num_map {
  char name[STRLEN];
  int type;
} num_map;

num_map folders[1000];

int print_menu(char *wd) {
  struct dirent **entries;
  int n = scandir(wd, &entries, NULL, alphasort);

  free(entries[0]);
  for (int j = 1; j < n; j++) {
    printw("%d: %s\n", j, entries[j]->d_name);
    strcpy(folders[j].name, entries[j]->d_name);
    free(entries[j]);
  }
  free(entries);
  refresh();
  return 0;
}

void logic(char *wd, char *prev_dir) {
  char *option = calloc(STRLEN, 1);
  strcpy(option, "-1");

  // file menu loop
  while (*option != 'q') {
    // print menu
    print_menu(wd);

    // take input from user
    printw("select the number of the folder you want to go to (0 to open nvim "
           "here)\n");
    int PATH_LENGTH = STRLEN;
    getnstr(option, PATH_LENGTH);
    clear();

    char *cd;

    asprintf(&cd, "%s/%s", wd, folders[atoi(option)].name);
    printw("current dir -- %s\n", cd);

    // options to open terminal and file
    char *command;
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
    } else if (*option == '0') {
      char *command;
      char *cd_into_dir;
      // concact strings (command + path)
      // asprintf(&cd_into_dir, "cd %s", wd);
      chdir(wd);
      asprintf(&command, "nvim %s", wd);
      system(cd_into_dir);
      system(command);
      system("exit");
      break;
    }

    // prev dir for fallback incase the option selected is a file
    prev_dir = wd;
    wd = cd;
  }
  free(option);
  clear();
}

int main(int argc, char *argv[]) {

  initscr(); /* Start curses mode 		  */
  raw();
  keypad(stdscr, TRUE);

  char *wd;
  char *prev_dir;
  if (argc == 2) {
    wd = argv[1];
    prev_dir = wd;
  } else {
    wd = "/home/neel/code";
    prev_dir = wd;
  }
  logic(wd, prev_dir);

  endwin(); /* End curses mode		  */
  return 0;
}
