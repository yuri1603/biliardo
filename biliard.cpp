#include "biliard.hpp"

#include <cmath>
#include <stdexcept>

namespace bl {

Point collision(const Path &r1, const Path &r2) {
  if (r1.slope == r2.slope) {
    throw std::runtime_error("Paths are parallel"); 
  }
  double x1 = (r1.y_intercept - r2.y_intercept) / (r2.slope - r1.slope);
  double x2 = r1.slope * x1 + r1.y_intercept;
  return Point{x1, x2};
}

Point first_collision(Path const &r1, Path const &r2, Path const &r3) {
  Point p1 = collision(r1, r2);
  Point p2 = collision(r1, r3);
  if (p1.x < 0) {
    return p2;
  }
  if (p2.x < 0) {
    return p1;
  }
  return p1.x < p2.x ? p1 : p2;
}

Path::Path(Point const &p1, Point const &p2) : slope{0.}, y_intercept{p2.y} {
  if (p1.y != p2.y) {
    slope = (p2.y - p1.y) / (p2.x - p1.x);
    y_intercept = p2.y - p2.x * slope;
  }
}

Path::Path(const Point &p1, double slope1)
    : slope{slope1}, y_intercept{p1.y - slope1 * p1.x} {}

Path::Path(double slope_, double y_intercept_)
    : slope{slope_}, y_intercept{y_intercept_} {}

Biliard::Biliard(double l, double y1, double y2)
    : upper_cushion({l, y2}, {0, y1}),
      lower_cushion({l, -y2}, {0., -y1}),
      lenght{l} {
  if (l <= 0.) {
    throw std::domain_error{"Biliard's lenght must be > 0"};
  }
  if (y1 <= 0. || y2 <= 0) {
    throw std::domain_error{"Biliard must be open"};
  }
}

Ball Biliard::Bounce(Ball const &b) {
  Path ball_path{b.start_point, b.slope};
  Point collision_point =
      first_collision(ball_path, lower_cushion, upper_cushion);
  if (collision_point.x > lenght) {
    return Ball{{lenght, ball_path.slope * lenght + ball_path.y_intercept},
                ball_path.slope};
  } else {
    throw std::runtime_error("Not implemented the bouncing mode yet");
  }
}
}  // namespace bl
