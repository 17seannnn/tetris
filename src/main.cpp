#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "curses.h"
#include "game.h"

int ask_difficult() {
    // We use buffer instead of character to get all input with line feed
    char buf[256];
    do {
        printf("Choose difficult: [1-3] ");
        fgets(buf, 256, stdin);
    } while (buf[0] != '1' && buf[0] != '2' && buf[0] != '3');
    return buf[0] - '0';
}

void init_program() {
    srand(time(0));
    curses.Init();
}

int main() {
    int difficult;
    try {
        difficult = ask_difficult();
        init_program();
        for (;;) {
            Game game(difficult);
            if (!game.Start())
                break;
        }
        return 0;
    } catch (const char* s) {
        fprintf(stderr, "%s\n", s);
    }
    return 1;
}
