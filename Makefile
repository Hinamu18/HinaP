all:
	gcc src/main.c src/dir.c src/TUI.c -lncursesw -ldl -lm -lpthread -o HinaP
