#include <curses.h>

#include "curses.h"

Curses curses;

static const int min_screen_width  = 80;
static const int min_screen_height = 24;

Curses::~Curses() {
    delwin(border_win);
    delwin(game_win);
    delwin(next_win);
    delwin(score_win);
    endwin();
}

void Curses::Init() {
    // Basics
    initscr();
    if (has_colors()) {
        use_default_colors();
        start_color();
    }
    noecho();
    raw();
    curs_set(0);
    clear();

    // Windows
    int width, height;
    getmaxyx(stdscr, height, width);
    if (width < min_screen_width || height < min_screen_height) {
        endwin();
        throw "Increase your terminal size to play";
    }

    int border_x = (width - border_win_width) / 2;
    int border_y = (height - border_win_height) / 2;
    border_win = newwin(border_win_height, border_win_width,
                        border_x, border_y);

    int game_x = border_x + 1;
    int game_y = border_y;
    game_win   = newwin(game_win_height, game_win_width, game_x, game_y);

    int next_x = border_x - 8;
    int next_y = border_y + 6;
    next_win  = newwin(next_win_height, next_win_width, next_x, next_y);

    int score_x = border_x + border_win_width + 2;
    int score_y = next_y;
    score_win  = newwin(score_win_height, score_win_width, score_x, score_y);

    keypad(game_win, true);
    wtimeout(game_win, 0);
}

void Curses::Refresh() {
    wnoutrefresh(border_win);
    wnoutrefresh(game_win);
    wnoutrefresh(score_win);
    wnoutrefresh(next_win);
    doupdate();
}
