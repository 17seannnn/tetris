#include <curses.h>

#include "curses.h"

#include "game.h"

Game::Game() {
    score = 0;
    for (int y = 0; y < curses.game_win_height; y++)
        for (int x = 0; x < curses.game_win_width; x++)
            map[y][x] = 0;
}

bool Game::Start() {
    return false;
}

void Game::DisplayBorder() const {

}

void Game::DisplayGame() const {

}

void Game::DisplayNext() const {

}

void Game::DisplayScore() const {

}
