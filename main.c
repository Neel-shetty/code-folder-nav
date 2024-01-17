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

// void split_path_file(char **p, char **f, char *pf) {
//
//   /* Find last delimiter. */
//   char *z;
//   for (z = pf + strlen(pf); z >= pf; z--) {
//     if (*z == '/' || *z == '\\')
//       break;
//   }
//
//   if (z >= pf) {
//     /* There is a delimiter: construct separate
//        path and filename fragments. */
//     printw("--> %i\n", z - pf);
//     *p = malloc(z - pf + 1);
//     strncpy(*p, pf, z - pf);
//     (*p)[z - pf] = '\0';
//     *f = malloc(strlen(z));
//     strcpy(*f, z + 1);
//   } else {
//     /* There is no delimiter: the entire
//        string must be a filename. */
//     *p = NULL;
//     *f = malloc(strlen(pf) + 1);
//     strcpy(*f, pf);
//   }
// }

void print_menu(char *wd) {
  struct dirent **entries;
  int n = scandir(wd, &entries, NULL, alphasort);

  for (int j = 1; entries[j] != NULL; j++) {
    // printf("%lu   ", sizeof(entries) / sizeof(entries[0]));
    printw("%d: %s\n", j, entries[j]->d_name);
    strcpy(folders[j].name, entries[j]->d_name);
    // free(entries[j]);
  }
  refresh();
}

void logic(DIR *d, char *wd, char *prev_dir, char *command) {
  // char *option = malloc(256);
  int option;
  // strcpy(option, "-1");
  option = -1;

  // file menu loop
  // while (atoi(option) != 0) {
  while (option != 48) {
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
    option = getch();
    printw("\n%d\n", option);
    clear();
    // scanf("%s", option);

    // concat string (path + folder name)
    char *cd;
    // if (strcmp(folders[atoi(option)].name, "..") == 0) {
    //   char *token = strtok(wd, "/");
    //   char **path, **last_dir;
    //   split_path_file(path, last_dir, wd);
    //   printw("%s - %s", *path, *last_dir);
    //   while (token != NULL) {
    //     // printf("%s\n", token);
    //     asprintf(&cd, "%s/%s", cd, token);
    //     printw("%s\n", cd);
    //     token = strtok(NULL, "/");
    //   }
    // } else {

    // asprintf(&cd, "%s/%s", wd, folders[atoi(option)].name);
    asprintf(&cd, "%s/%s", wd, folders[option - 48].name);
    // }
    printw("current dir -- %s\n", cd);

    // options to open terminal and file
    // if (*option == 't') {
    //   // open dir in terminal
    //   asprintf(&command, "kitty --detach %s", wd);
    //   system(command);
    //   system("exit");
    //   break;
    // } else if (*option == 'o') {
    //   // open file
    //   asprintf(&command, "xdg-open %s 1>/dev/null 2>&1", wd);
    //   system(command);
    //   system("exit");
    //   break;
    // }

    // prev dir for fallback incase the option selected is a file
    prev_dir = wd;
    wd = cd;
    closedir(d);
  }

  // when quit
  // if (atoi(option) == 0) {
  if (option == 48) {
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
  clear();
  return;
}

int main(int argc, char *argv[]) {

  // init variables
  initscr(); /* Start curses mode 		  */
  // raw();
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
    // print_menu(d);
  }

  // option management
  endwin(); /* End curses mode		  */
  return 0;
}
