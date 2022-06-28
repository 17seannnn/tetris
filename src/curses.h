#ifndef COMMON_H
#define COMMON_H

#include <curses.h>

struct Curses {
    enum win_size {
        border_win_width  = 12,
        border_win_height = 17,

        game_win_width  = 10,
        game_win_height = 16,

        next_win_width  = 5,
        next_win_height = 6,

        score_win_width  = 20,
        score_win_height = 1,

        gameover_win_width = 20,
        gameover_win_height = 7,
    };

    enum color_pair {
        I_pair = 1,
        L_pair,
        J_pair,
        O_pair,
        S_pair,
        Z_pair,
        T_pair,

        gameover_pair
    };

    WINDOW* border_win;
    WINDOW* game_win;
    WINDOW* next_win;
    WINDOW* score_win;
    WINDOW* gameover_win;

    Curses() {}
    ~Curses();

    void Init();
    void Refresh();
};

extern Curses curses;

#endif
