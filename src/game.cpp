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
    Shape *current = get_random_shape();
    Shape *next = get_random_shape();

    DisplayBorder();
    DisplayGame();
    DisplayScore();
    DisplayNext(next);

    bool quit = false;
    while (!quit) {
        DisplayGame();
        current->Display();

        curses.Refresh();

        ch = wgetch(curses.game_win);
        switch (ch) {
        case KEY_F(1):
            delete current;
            current = next;
            next = get_random_shape();
            DisplayNext(next);
            break;
        case KEY_F(2):
            current->Reverse(0);
            break;
        case KEY_F(3):
            current->Reverse(1);
            break;
        case 'Q': case 'q':
            quit = true;
            break;
        }

        usleep(50000);
    }

    delete current;
    delete next;

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

void Game::DisplayNext(const Shape* shape) const {
    wclear(curses.next_win);
    mvwprintw(curses.next_win, 0, 0, "Next:");
    shape->Display(curses.next_win, 0, 2);
}

void Game::DisplayScore() const {
    wclear(curses.score_win);
    mvwprintw(curses.score_win, 0, 0, "Score: %d", score);
}
