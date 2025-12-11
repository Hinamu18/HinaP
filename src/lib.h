#ifndef lib_h
#define lib_h

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <ncurses.h>
#include <stdbool.h>
#include <locale.h>

#include "miniaudio.h"

extern char *the_dir_path; 
extern float volume;
extern bool audio_pause;
extern ma_device device;

void list_files();
char* draw_files_menu(); 
int audio_ui(char* audio_name,ma_decoder* decoder);
void draw_cover(const char* title);
double get_audio_length(ma_decoder* decoder);


#endif
