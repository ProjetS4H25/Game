struct Rect {
  int x;
  int y;
  int w;
  int h;
};

struct Rect createRect(int x, int y, int w, int h) {
  struct Rect rect = {0};
  rect.x = x;
  rect.y = y;
  rect.w = w;
  rect.h = h;
  return rect;
}