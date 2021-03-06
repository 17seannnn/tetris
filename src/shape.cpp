#include <stdlib.h>
#include <curses.h>

#include "curses.h"

#include "shape.h"

Shape* Shape::GetRandomShape(shape_id exclude) {
    Shape* shape;

    int id;
    do {
        id = rand() % shape_count + 1; // + 1 because shape id starts with 1
    } while (id == exclude);

    switch (id) {
    case I_id:
        shape = new I_Shape();
        break;
    case L_id:
        shape = new L_Shape();
        break;
    case J_id:
        shape = new J_Shape();
        break;
    case O_id:
        shape = new O_Shape();
        break;
    case S_id:
        shape = new S_Shape();
        break;
    case Z_id:
        shape = new Z_Shape();
        break;
    case T_id:
        shape = new T_Shape();
        break;
    default:
        return 0;
    }

    // Reverse this shape 0..3 times
    for (int i = rand() % 4; i != 0; i--)
        shape->Reverse();

    return shape;
}

Shape::Shape(shape_id sid, int c)
    : id(sid), ch(c), pos_x((curses.game_win_width - 4) / 2), pos_y(0) {
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

bool Shape::Move(const int (*map)[curses.game_win_width], int dx, int dy) {
    int new_x = pos_x + dx, new_y = pos_y + dy;

    if (CheckCollision(map, shape, new_x, new_y))
        return false;

    pos_x = new_x;
    pos_y = new_y;

    return true;
}

void Shape::Reverse(const int (*map)[curses.game_win_width], int side) {
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

    if (map && CheckCollision(map, new_shape, pos_x, pos_y))
        return;

    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            shape[y][x] = new_shape[y][x];
}

void Shape::Place(int (*map)[curses.game_win_width]) const {
    for (int y = 0; y < 4; y++)
        for (int x = 0; x < 4; x++)
            if (shape[y][x])
                map[pos_y+y][pos_x+x] = shape[y][x];
}

int Shape::Push(const int (*map)[curses.game_win_width]) {
    int count = 0;
    while (Move(map, 0, 1)) {
        count++;
    }
    return count;
}

bool Shape::CheckCollision(const int (*map)[curses.game_win_width],
                           const int shape[4][4],
                           int new_x, int new_y) {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 4; x++) {
            if (!shape[y][x])
                continue;

            // Check borders
            if (new_x + x < 0 || new_x + x >= curses.game_win_width ||
                new_y + y >= curses.game_win_height)
                return true;

            // Check other bricks
            if (map[new_y+y][new_x+x])
                return true;
        }
    }

    return false;
}
