#ifndef SHAPE_H
#define SHAPE_H

class Shape {
public:
    enum shape_id {
        None_id = 0,
        I_id,
        L_id,
        J_id,
        O_id,
        S_id,
        Z_id,
        T_id,
    };
private:
    enum { shape_count = 7 };

    shape_id id;
    int ch;
    int pos_x, pos_y;
    int shape[4][4];
public:
    Shape(shape_id sid, int c);
    Shape(const Shape& tocopy);
    const Shape& operator=(const Shape& tocopy);

    static Shape* GetRandomShape(shape_id exclude = None_id);

    shape_id GetId() const
        { return id; }
    int GetX() const
        { return pos_x; }
    int GetY() const
        { return pos_y; }
    // Just .Display() to display on game window on current position
    void Display(WINDOW* win = curses.game_win,
                 int on_x = -1, int on_y = -1) const;
    // False on collision with other bricks
    bool Move(const int (*map)[curses.game_win_width], int dx, int dy);
    // side == 1 - turn right, 0 - turn left
    // Just .Reverse() if we don't want check map
    void Reverse(const int (*map)[curses.game_win_width] = 0, int side = 0);
    void Place(int (*map)[curses.game_win_width]) const;
    void Push(const int (*map)[curses.game_win_width]);
protected:
    void Add(int x, int y)
        { shape[y][x] = ch; }

    // True on collision with something on map
    static bool CheckCollision(const int (*map)[curses.game_win_width],
                               const int shape[4][4],
                               int new_x, int new_y);
};

class I_Shape : public Shape {
public:
    I_Shape() : Shape(I_id, ' ' | COLOR_PAIR(curses.I_pair)) {
        Add(1, 0);
        Add(1, 1);
        Add(1, 2);
        Add(1, 3);
    }
};

class L_Shape : public Shape {
public:
    L_Shape() : Shape(L_id, ' ' | COLOR_PAIR(curses.L_pair)) {
        Add(1, 1);
        Add(1, 2);
        Add(1, 3);
        Add(2, 3);
    }
};

class J_Shape : public Shape {
public:
    J_Shape() : Shape(J_id, ' ' | COLOR_PAIR(curses.J_pair)) {
        Add(2, 1);
        Add(2, 2);
        Add(2, 3);
        Add(1, 3);
    }
};

class O_Shape : public Shape {
public:
    O_Shape() : Shape(O_id, ' ' | COLOR_PAIR(curses.O_pair)) {
        Add(1, 2);
        Add(2, 2);
        Add(1, 3);
        Add(2, 3);
    }
};

class S_Shape : public Shape {
public:
    S_Shape() : Shape(S_id, ' ' | COLOR_PAIR(curses.S_pair)) {
        Add(0, 2);
        Add(1, 2);
        Add(1, 3);
        Add(2, 3);
    }
};

class Z_Shape : public Shape {
public:
    Z_Shape() : Shape(Z_id, ' ' | COLOR_PAIR(curses.Z_pair)) {
        Add(3, 2);
        Add(2, 2);
        Add(2, 3);
        Add(1, 3);
    }
};

class T_Shape : public Shape {
public:
    T_Shape() : Shape(T_id, ' ' | COLOR_PAIR(curses.T_pair)) {
        Add(0, 3);
        Add(1, 3);
        Add(2, 3);
        Add(1, 2);
    }
};

#endif
