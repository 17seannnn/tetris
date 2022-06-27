#ifndef GAME_H
#define GAME_H

#include "shape.h"
#include "curses.h"

class Game {
    int score;
    int map[curses.game_win_height][curses.game_win_width];
public:
    Game();
    ~Game() {}

    bool Start(); // True on play again
private:
    void DisplayAll(const Shape* current, const Shape* next) const;
    void DisplayBorder() const;
    void DisplayGame() const;
    void DisplayNext(const Shape* shape) const;
    void DisplayScore() const;
};

#endif
