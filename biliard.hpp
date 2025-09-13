#ifndef BILIARD_HPP
#define BILIARD_HPP

#include <vector>

namespace bl {

struct Point {
  double x{0.};
  double y{0.};
};

struct Ball {
  double y_coord{0.};
  double angle{0.};
};

struct Sample {
  std::vector<double> angles;
  std::vector<double> y_coord;
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

Path Bounce(Path const& r1, Path const& r2);

class Biliard {
  Path upper_cushion_;
  Path lower_cushion_;
  double lenght_;

 public:
  Biliard(double l, double y1, double y2);

  void Dynamic(Ball& b);

  Sample split(std::vector<Ball> &balls);

  std::vector<Ball> random_balls(long unsigned int N);
};

}  // namespace bl

#endif