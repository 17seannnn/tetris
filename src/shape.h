#ifndef SHAPE_H
#define SHAPE_H

class Shape {
    int ch;
    int pos_x, pos_y;
    int shape[4][4];
public:
    Shape(int c);
    Shape(const Shape& tocopy);
    const Shape& operator=(const Shape& tocopy);

    int GetX() const
        { return pos_x; }
    int GetY() const
        { return pos_y; }
    // just .Display() to display on game window on current position
    void Display(WINDOW* win = curses.game_win,
                 int on_x = -1, int on_y = -1) const;
    void Hide() const;
    void Move(int dx, int dy);
    void Reverse(int side); // 1 - turn right, 0 - turn left
protected:
    void Add(int x, int y)
        { shape[y][x] = ch; }
};

class I_Shape : public Shape {
public:
    I_Shape() : Shape('#') {
        Add(1, 0);
        Add(1, 1);
        Add(1, 2);
        Add(1, 3);
    }
};

class L_Shape : public Shape {
public:
    L_Shape() : Shape('#') {
        Add(1, 1);
        Add(1, 2);
        Add(1, 3);
        Add(2, 3);
    }
};

class J_Shape : public Shape {
public:
    J_Shape() : Shape('#') {
        Add(2, 1);
        Add(2, 2);
        Add(2, 3);
        Add(1, 3);
    }
};

class O_Shape : public Shape {
public:
    O_Shape() : Shape('#') {
        Add(1, 2);
        Add(2, 2);
        Add(1, 3);
        Add(2, 3);
    }
};

class S_Shape : public Shape {
public:
    S_Shape() : Shape('#') {
        Add(0, 2);
        Add(1, 2);
        Add(1, 3);
        Add(2, 3);
    }
};

class Z_Shape : public Shape {
public:
    Z_Shape() : Shape('#') {
        Add(3, 2);
        Add(2, 2);
        Add(2, 3);
        Add(1, 3);
    }
};

class T_Shape : public Shape {
public:
    T_Shape() : Shape('#') {
        Add(0, 3);
        Add(1, 3);
        Add(2, 3);
        Add(1, 2);
    }
};

Shape* get_random_shape();

#endif
