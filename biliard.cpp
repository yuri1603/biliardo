#include "biliard.hpp"
#include <stdexcept>

bl::Path bl::pts_path(const Point &p1, const Point &p2) {
  if (p1.y == p2.y) {
    return {0, p2.y};
  }
  double m = (p2.y - p1.y) / (p2.x - p1.x);
  double q = p2.y - p2.x * m;
  return {m, q};
}

bl::Path bl::slp_path(const Point &p1, double slope1) {
  double q = p1.y - slope1 * p1.x;
  return {slope1, q};
}

bl::Point bl::collision(const Path &r1, const Path &r2) {
    if (r1.slope == r2.slope) {
        throw std::runtime_error("No intersection point");
    }
  double x1 = (r1.y_intecept - r2.y_intecept) / (r2.slope - r1.slope);
  double x2 = (r2.slope * r1.y_intecept - r1.slope * r2.y_intecept) /
              (r2.slope - r1.slope);
  return {x1, x2};
}
