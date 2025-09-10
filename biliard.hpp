#ifndef BILIARD_HPP
#define BILIARD_HPP

namespace bl {

struct Point {
  double x{0.};
  double y{0.};
};

struct Ball {
  Point start_point{};
  double slope{0.};
};

struct Path {
  double slope{0.};
  double y_intercept{0.};
  Path(double slope, double y_intercept);
  Path(Point const& p1, Point const& p2);
  Path(Point const& p1, double slope1);
};

Point collision(
    Path const& r1,
    Path const& r2);  // prende due rette e restituisce il punto di intersezione

Point first_collision(Path const& r1, Path const& r2, Path const& r3);

class Biliard {
  Path upper_cushion;
  Path lower_cushion;
  double lenght;

 public:
  Biliard(double l, double y1, double y2);

  Ball Bounce(Ball const& b);
};

}  // namespace bl

#endif