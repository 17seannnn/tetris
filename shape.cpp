#include <curses.h>

#include "curses.h"

#include "shape.h"

Shape::Shape(int c)
    : ch(c), pos_x((curses.game_win_width - 4) / 2), pos_y(0) {
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            shape[y][x] = 0;
}

Shape::Shape(const Shape& tocopy) {
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            shape[y][x] = tocopy.shape[y][x];
}

const Shape& Shape::operator=(const Shape& tocopy) {
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            shape[y][x] = tocopy.shape[y][x];
    return *this;
}

void Shape::Display() const {
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            if (shape[y][x])
                mvwaddch(curses.game_win, pos_y + y, pos_x + x, ch);
}

void Shape::Hide() const {
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            mvwaddch(curses.game_win, pos_y + y, pos_x + x, '.');
}
