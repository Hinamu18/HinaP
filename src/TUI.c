#include "lib.h"
#include <ncurses.h>

void draw_cover(const char* title){
  int rows, cols;
  getmaxyx(stdscr, rows, cols); 
  box(stdscr, 0, 0);
  mvprintw(0, 2, " %s ", title);
}

int audio_ui(char* audio_name,ma_decoder* decoder){
  initscr();
  cbreak();            
  noecho();
  keypad(stdscr, TRUE); 
  curs_set(0);

  timeout(100);

  int ch;
  bool running = true;
  
  double total_time = get_audio_length(decoder);
  double current_time_sec = 0;
  double current_time = 0;
  int cur_min; 
  int cur_sec; 
  int tot_min; 
  int tot_sec; 

  ma_uint64 current_frame=0;
  while (running) {
    ma_decoder_get_cursor_in_pcm_frames(decoder, &current_frame);
    
    if (decoder->outputSampleRate > 0) {
      current_time_sec = (double)current_frame / (double)decoder->outputSampleRate;
    }

    if (total_time > 0) {
      current_time = current_time_sec / total_time;
    }

    clear();
    draw_cover("[ Playing Audio ]");

    // center text roughly
    mvprintw(2, 4, "File: %s", audio_name);
    mvprintw(3, 4, "Controls:");
    if (audio_pause) mvprintw(4, 6, "[Space] Resume");
    else mvprintw(4, 6, "[Space] Stop");
    mvprintw(5, 6, "[Up] Volume Down");
    mvprintw(6, 6, "[Down] Volume Up");
    mvprintw(7, 6, "[->] skip 3s");
    mvprintw(8, 6, "[<-] back 3s");
    mvprintw(9, 6, "[D]  Change Audio");
    mvprintw(10, 6, "[Q]  Quit");


    mvprintw(11, 4, "Volume: %.0f%%", volume * 100);


    cur_min = (int)current_time_sec / 60;
    cur_sec = (int)current_time_sec % 60;
    tot_min = (int)total_time / 60;
    tot_sec = (int)total_time % 60;

    mvprintw(13, 4, "Time: %02d:%02d / %02d:%02d", cur_min, cur_sec, tot_min, tot_sec);

    int bar_width = 40;
    float progress = 0.0f;
    if (total_time > 0) progress = (float)(current_time_sec / total_time);
    
    int filled = (int)(progress * bar_width);

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

      case ' ':
        audio_pause = !audio_pause;
        if (audio_pause) ma_device_stop(&device);
        else ma_device_start(&device);
        break;
      case 'd':
        return 1;
        break;
    }
  }
  endwin();
  return -1;
}
