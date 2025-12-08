#include "lib.h"

#define MAX_FILES 256

char* files[MAX_FILES];
int total_files = 0;

void list_files(){
  DIR *dir;
  struct dirent *entry;

  dir = opendir(the_dir_path);
  if (dir == NULL) return;

  int i = 0;
  while ((entry = readdir(dir)) != NULL) {
    // skip hidden files/dirs (. and ..)
    if(entry->d_name[0] == '.') continue;

    if (i >= MAX_FILES) break;

    files[i] = strdup(entry->d_name);
    i++;
  }
  total_files = i;
  closedir(dir);
}

char* draw_files_menu(){
  if (total_files == 0) return NULL;

  initscr();
  cbreak();            
  noecho();
  keypad(stdscr, TRUE); 
  curs_set(0);

  int highlight = 0;   
  int ch;
  char* selected = NULL;

  while (true) {
    clear();
    draw_cover("[ Select File ]");

    int start_y = 2;

    // draw the list
    for (int i = 0; i < total_files; i++) {
      if (i == highlight) {
        attron(A_REVERSE); 
        mvprintw(start_y + i, 2, " %s ", files[i]);
        attroff(A_REVERSE);
      } else {
        mvprintw(start_y + i, 2, " %s ", files[i]);
      }
    }

    refresh();

    ch = getch();
    switch (ch) {
      case KEY_UP:
        highlight--;
        if (highlight < 0) highlight = total_files - 1;
        break;

      case KEY_DOWN:
        highlight++;
        if (highlight >= total_files) highlight = 0;
        break;

      case '\n':
        selected = files[highlight];
        goto end;

      case 'q':
      case 'Q':
        selected = NULL;
        goto end;
    }
  }

end:
  endwin();
  return selected;
}
