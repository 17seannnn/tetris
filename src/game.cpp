#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#include <curses.h>

#include "curses.h"
#include "shape.h"

#include "game.h"

enum {
    fall_delay = 400000,

    score_push_mult = 2,
    score_line_1 = 100,
    score_line_2 = 200,
    score_line_3 = 400,
    score_line_4 = 800,
};

static const char msg_gameover[] = "Game Over...";
static const char msg_playagain[] = "Play again?";
static const char msg_playagain_choice[] = "[y/n/q]";

Game::Game() {
    score = 0;
    for (int y = 0; y < curses.game_win_height; y++)
        for (int x = 0; x < curses.game_win_width; x++)
            map[y][x] = 0;
}

bool Game::IsOver() const {
    for (int x = 0; x < curses.game_win_width; x++)
        if (map[3][x])
            return true;
    return false;
}

static void next_shape(Shape*& current, Shape*& next) {
    if (current)
        delete current;
    current = next;
    next = Shape::GetRandomShape(current->GetId());
}

static int get_diff(timeval& tv) {
    long diff;
    timeval cur;

    gettimeofday(&cur, 0);
    if (cur.tv_usec >= tv.tv_usec)
        diff = cur.tv_usec - tv.tv_usec;
    else
        diff = (1000000 - tv.tv_usec) + cur.tv_usec;

    return diff;
}

bool Game::Start() {
    int ch;
    bool redraw;
    bool can_fall = true;
    timeval last_fall;
    Shape* current = 0;
    Shape* next = Shape::GetRandomShape();

    next_shape(current, next);
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
        case ' ':
            score += current->Push(map) * score_push_mult;
            break;
        case 'Q': case 'q':
            quit = true;
            break;
        default:
            break;
        }
        if (ch != ERR)
            redraw = true;

        if (can_fall) {
            if (!current->Move(map, 0, 1)) {
                current->Place(map);
                CheckLines();
                next_shape(current, next);
            } else {
                can_fall = false;
            }
            redraw = true;
            gettimeofday(&last_fall, 0);
        }

        if (redraw) {
            DisplayAll(current, next);
            redraw = false;
        }

        if (IsOver())
            break;

        if (get_diff(last_fall) >= fall_delay)
            can_fall = true;
    }

    DisplayGameOver();
    do {
        ch = wgetch(curses.gameover_win);
    } while (ch != 'Y' && ch != 'y' &&
             ch != 'N' && ch != 'n' &&
             ch != 'Q' && ch != 'q');

    delete current;
    delete next;

    return ch == 'Y' || ch == 'y';
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

void Game::DisplayGameOver() const {
    wattrset(curses.game_win, A_BLINK);
    DisplayGame();
    wattrset(curses.game_win, A_NORMAL);
    curses.Refresh();

    wclear(curses.gameover_win);
    wattrset(curses.gameover_win, A_REVERSE);

    int x = (curses.gameover_win_width - strlen(msg_gameover))/2;
    int y = curses.gameover_win_height/2 - 1;
    mvwprintw(curses.gameover_win, y, x, msg_gameover);
    x = (curses.gameover_win_width - strlen(msg_playagain))/2;
    mvwprintw(curses.gameover_win, y+1, x, msg_playagain);
    x = (curses.gameover_win_width - strlen(msg_playagain_choice))/2;
    mvwprintw(curses.gameover_win, y+2, x, msg_playagain_choice);

    wattrset(curses.gameover_win, A_NORMAL);

    wrefresh(curses.gameover_win);
}

void Game::CheckLines() {
    int x, y;
    int count = 0;

    for (y = 0; y < curses.game_win_height; y++) {
        for (x = 0; x < curses.game_win_width; x++)
            if (!map[y][x])
                break;
        if (x < curses.game_win_width)
            continue;
        for ( ; y > 0; y--)
            for (x = 0; x < curses.game_win_width; x++)
                map[y][x] = map[y-1][x];
        count++;
    }

    // Player can fill <= 4 lines per time
    switch (count) {
    case 1:
        score += score_line_1;
        break;
    case 2:
        score += score_line_2;
        break;
    case 3:
        score += score_line_3;
        break;
    case 4:
        score += score_line_4;
        break;
    default:
        break;
    }
}
