#include <unistd.h>
#include <curses.h>

#include "curses.h"
#include "shape.h"

#include "game.h"

Game::Game() {
    score = 0;
    for (int y = 0; y < curses.game_win_height; y++)
        for (int x = 0; x < curses.game_win_width; x++)
            map[y][x] = 0;
}

bool Game::Start() {
    int ch;
    Shape *shape = new Square();

    DisplayBorder();
    DisplayScore();
    DisplayNext();
    bool quit = false;
    while (!quit) {
        DisplayGame();
        shape->Display();
        curses.Refresh();

        ch = wgetch(curses.game_win);
        switch (ch) {
        case 'Q': case 'q':
            quit = true;
            break;
        }

        usleep(50000);
    }

    delete shape;
    return false;
}

void Game::DisplayBorder() const {
    int ch = ' ' | A_REVERSE;
    int y;
    for (y = 0; y < curses.border_win_height-1; y++) {
        mvwaddch(curses.border_win, y, 0, ch);
        mvwaddch(curses.border_win, y, curses.border_win_width-1, ch);
    }
    wmove(curses.border_win, y, 0);
    for (int x = 0; x < curses.border_win_width; x++)
        waddch(curses.border_win, ch);
}

void Game::DisplayGame() const {
    for (int y = 0; y < curses.game_win_height; y++)
        for (int x = 0; x < curses.game_win_width; x++)
            mvwaddch(curses.game_win, y, x, map[y][x] ? map[y][x] : '.');
}

void Game::DisplayNext() const {
    mvwprintw(curses.next_win, 0, 0, "Next:");
    for (int i = 0; i < 4; i++)
        mvwprintw(curses.next_win, i+2, 0, "####");
}

void Game::DisplayScore() const {
    mvwprintw(curses.score_win, 0, 0, "Score: %d", score);
}
