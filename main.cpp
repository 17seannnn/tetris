#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "curses.h"
#include "game.h"

void init_program() {
    srand(time(0));
    curses.Init();
}

int main() {
    try {
        init_program();
        for (;;) {
            Game game;
            if (!game.Start())
                break;
        }
        return 0;
    } catch (const char* s) {
        fprintf(stderr, "%s\n", s);
    }
    return 1;
}
