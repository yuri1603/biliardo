#ifndef BILIARD_HPP
#define BILIARD_HPP

#include <vector>

namespace bl {

struct Point {
  double x{0.};
  double y{0.};
};

struct Cushion {
  Point right{0.};
  Point left{0.};
};

struct Result {
  double final_angle{0.};
  double final_intercept{0.};
};

struct Ball {
  Point start_point{0.};
  double start_angle{0.};
};

struct Path {
  double slope{0.};
  double y_intecept{0.};
};

Path pts_path(
    Point const& p1,
    Point const& p2);  // prende due punti e restituisce l'equazione della retta

Path slp_path(Point const& p1,
              double slope1);  // prende un punto e la slope e restituisce
                               // l'equazione della retta

Point collision(
    Path const& r1,
    Path const& r2);  // prende due rette e restituisce il punto di intersezione

class Biliard {
  Cushion upper;
  Cushion lower;
  std::vector<Ball> entries;

 public:
};

}  // namespace bl

#endif