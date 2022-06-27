#include <stdlib.h>
#include <curses.h>

#include "curses.h"

#include "shape.h"

Shape* get_random_shape() {
    // We have 7 different shapes
    switch (rand() % 7) {
    case 0:
        return new I_Shape();
    case 1:
        return new L_Shape();
    case 2:
        return new J_Shape();
    case 3:
        return new O_Shape();
    case 4:
        return new S_Shape();
    case 5:
        return new Z_Shape();
    case 6:
        return new T_Shape();
    default:
        return 0;
    }
}

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

void Shape::Display(WINDOW* win, int on_x, int on_y) const {
    if (on_x == -1) {
        for (int y = 0; y < 4; y++)
            for (int x = 0; x < 4; x++)
                if (shape[y][x])
                    mvwaddch(win, pos_y + y, pos_x + x, ch);
    } else {
        for (int y = 0; y < 4; y++)
            for (int x = 0; x < 4; x++)
                if (shape[y][x])
                    mvwaddch(win, on_y + y, on_x + x, ch);
    }
}

void Shape::Hide() const {
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            if (shape[y][x])
                mvwaddch(curses.game_win, pos_y + y, pos_x + x, '.');
}

void Shape::Reverse(int side) {
    int new_shape[4][4];

    if (side == 0) {
        for (int y = 0; y < 4; y++)
            for (int x = 0; x < 4; x++)
                new_shape[y][x] = shape[x][3-y];
    } else {
        for (int y = 0; y < 4; y++)
            for (int x = 0; x < 4; x++)
                new_shape[y][x] = shape[3-x][y];
    }

    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            shape[y][x] = new_shape[y][x];
}
