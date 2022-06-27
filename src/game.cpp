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

static void next_shape(Shape*& current, Shape*& next) {
    delete current;
    current = next;
    next = Shape::GetRandomShape();
}

bool Game::IsOver() const {
    for (int x = 0; x < curses.game_win_width; x++)
        if (map[3][x])
            return true;
    return false;
}

bool Game::Start() {
    int ch;
    Shape* current = Shape::GetRandomShape();
    Shape* next = Shape::GetRandomShape();

    DisplayAll(current, next);

    bool quit = false;
    while (!quit) {
        ch = wgetch(curses.game_win);
        switch (ch) {
        case 'N': case 'n':
            next_shape(current, next);
            break;
        case 'P': case 'p':
            current->Place(map);
            next_shape(current, next);
            break;
        case KEY_UP:
            current->Reverse(map, 1);
            break;
        case KEY_DOWN:
            current->Reverse(map, 0);
            break;
        case KEY_LEFT:
            current->Move(map, -1, 0);
            break;
        case KEY_RIGHT:
            current->Move(map, 1, 0);
            break;
        case 'Q': case 'q':
            quit = true;
            break;
        default:
            break;
        }

        if (!current->Move(map, 0, 1)) {
            current->Place(map);
            next_shape(current, next);
        }

        if (IsOver())
            break;

        DisplayAll(current, next);

        usleep(200000);
    }

    delete current;
    delete next;

    return false;
}

void Game::DisplayAll(const Shape* current, const Shape* next) const {
    DisplayBorder();
    DisplayGame();
    DisplayScore();
    DisplayNext(next);
    current->Display();
    curses.Refresh();
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
