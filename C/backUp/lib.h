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

// variables
extern char *the_dir_path; 
extern float volume;

// functions
void list_files();
char* draw_files_menu(); 
void audio_ui(char* audio_name);
void draw_cover(const char* title);


#endif
