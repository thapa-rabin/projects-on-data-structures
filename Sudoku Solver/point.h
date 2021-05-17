#ifndef _POINT_H
#define _POINT_H

using namespace std;

class point {
  private:
    int x, y;

  public:
    point(int x_val, int y_val) {
      x = x_val;
      y = y_val;
    }

    ~point() { }

    //getters
    int xValue() {
      return x;
    }

    int yValue() {
      return y;
    }
};

#endif
