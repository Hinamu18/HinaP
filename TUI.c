#include "lib.h"
#include <ncurses.h>

void draw_cover(const char* title){
  int rows, cols;
  getmaxyx(stdscr, rows, cols); 
  box(stdscr, 0, 0);
  mvprintw(0, 2, " %s ", title);
}

int audio_ui(char* audio_name){
  initscr();
  cbreak();            
  noecho();
  keypad(stdscr, TRUE); 
  curs_set(0);


  int ch;
  bool running = true;

  while (running) {
    clear();
    draw_cover("[ Playing Audio ]");

    // center text roughly
    mvprintw(2, 4, "File: %s", audio_name);
    mvprintw(4, 4, "Controls:");
    mvprintw(5, 6, "[Up] Volume Down");
    mvprintw(6, 6, "[Down] Volume Up");
    mvprintw(7, 6, "[->] skip 3s");
    mvprintw(8, 6, "[<-] back 3s");
    mvprintw(9, 6, "[D]  Change Audio");
    mvprintw(10, 6, "[Q]  Quit");

    // volume bar
    mvprintw(11, 4, "Volume: %.0f%%", volume * 100);
    

    int bar_width = 40;
    int filled = (int)(volume * bar_width);

    mvprintw(12, 4, "[");
    for (int i = 0; i < bar_width; i++){
      if (i < filled) addch('#');
      else addch('-');
    }
    addch(']');


    refresh();

    // input 
    ch = getch();
    switch (ch) {
      case 'q':
      case 'Q':
        endwin();
        return -1;
      case KEY_DOWN:
        if (volume > 0.0f) volume -= 0.05f;
        if (volume < 0.0f) volume = 0.0f;
        break;
      case KEY_UP:
        if (volume < 1.0f) volume += 0.05f;
        if (volume > 1.0f) volume = 1.0f;
        break;
      case KEY_RIGHT:
      break;
      case KEY_LEFT:
      break;
      case 'd':
        return 1;
      break;
    }
  }
  endwin();
  return -1;
}
