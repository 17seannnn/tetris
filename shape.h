#include <curses.h>

class Shape {
    int ch;
    int pos_x, pos_y;
    int shape[4][4];
public:
    Shape(int c);
    Shape(const Shape& tocopy);

    const Shape& operator=(const Shape& tocopy);

    int GetX()
        { return pos_x; }
    int GetY()
        { return pos_y; }
    void Display() const;
    void Hide() const;
    void Move(int dx, int dy);
    void Reverse(int side); // 1 - turn right, 0 - turn left
protected:
    void Add(int x, int y)
        { shape[y][x] = ch; }
};

class Square : public Shape {
public:
    Square() : Shape('#') {
        Add(1, 2);
        Add(2, 2);
        Add(1, 3);
        Add(2, 3);
    }
};
