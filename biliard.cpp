#include "biliard.hpp"

#include <cmath>
#include <stdexcept>

namespace bl {

Path pts_path(const Point &p1, const Point &p2) {
  if (p1.y == p2.y) {
    return Path{0, p2.y};
  }
  double m = (p2.y - p1.y) / (p2.x - p1.x);
  double q = p2.y - p2.x * m;
  return Path{m, q};
}

Path slp_path(const Point &p1, double slope1) {
  double q = p1.y - slope1 * p1.x;
  return Path{slope1, q};
}

Point collision(const Path &r1, const Path &r2) {
  double x1 = (r1.y_intercept - r2.y_intercept) / (r2.slope - r1.slope);
  double x2 = r1.slope * x1 + r1.y_intercept;
  return Point{x1, x2};
}

Point first_collision(Point const &p1, Point const &p2) {
  if (p1.x < 0) {
    return p2;
  }
  if (p2.x < 0) {
    return p1;
  }
  return p1.x < p2.x ? p1 : p2;
}

Biliard::Biliard(double l, double y1, double y2)
    : upper{{l, y2}, {0., y1}}, lower{{l, -y2}, {0., -y1}} {
  if (l <= 0.) {
    throw std::domain_error{"Biliard's lenght must be > 0"};
  }
  if (y1 <= 0. || y2 <= 0) {
    throw std::domain_error{"Biliard must be open"};
  }
}

Ball Biliard::Bounce(Ball const &b) {
  Path up_cush = pts_path(upper.right, upper.left);
  Path low_cush = pts_path(lower.right, lower.left);
  Path ball_path = slp_path(b.start_point, b.slope);
  Point first_coll1 = collision(up_cush, ball_path);
  Point first_coll2 = collision(low_cush, ball_path);
  Point first_pt_coll = first_collision(first_coll1, first_coll2);
  if (first_pt_coll.x > upper.right.x) {
    return Ball{{upper.right.x,
                 ball_path.slope * upper.right.x + ball_path.y_intercept},
                ball_path.slope};
  } else {
    
  }
}
}  // namespace bl