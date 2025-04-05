struct Rect {
  int x;
  int y;
  int w;
  int h;
};

struct Rect Rect(int x, int y, int w, int h) {
  struct Rect rect;
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  return rect;
}