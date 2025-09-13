#include "biliard.hpp"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace bl {

Point collision(const Path &r1, const Path &r2) {
  if (r1.slope == r2.slope) {
    return Point{-1., 0.};
  }
  double x = (r1.y_intercept - r2.y_intercept) / (r2.slope - r1.slope);
  double y = r1.slope * x + r1.y_intercept;
  return Point{x, y};
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
    : upper_cushion_({l, y2}, {0, y1}),
      lower_cushion_({l, -y2}, {0., -y1}),
      lenght_{l} {
  if (l <= 0.) {
    throw std::domain_error{"Biliard's lenght must be > 0"};
  }
  if (y1 <= 0. || y2 <= 0) {
    throw std::domain_error{"Biliard must be open"};
  }
}

Path Bounce(Path const &r1, Path const &r2) {
  Point collision_point = collision(r1, r2);
  double new_slope =
      (r1.slope * std::pow(r2.slope, 2) - r1.slope + 2 * r2.slope) /
      (1 - std::pow(r2.slope, 2) + 2 * r2.slope * r1.slope);
  return Path{collision_point, new_slope};
}

void Biliard::Dynamic(Ball &b) {
  Path ball_path{{0., b.y_coord}, std::tan(b.angle)};
  Point collision_point =
      first_collision(ball_path, lower_cushion_, upper_cushion_);
  while (collision_point.x <= lenght_ && collision_point.x >= 0) {
    if (collision_point.y > 0) {
      ball_path = Bounce(ball_path, upper_cushion_);
      collision_point = collision(ball_path, lower_cushion_);
    } else if (collision_point.y < 0) {
      ball_path = Bounce(ball_path, lower_cushion_);
      collision_point = collision(ball_path, upper_cushion_);
    }
  }
  if (upper_cushion_.slope < 0 &&
      (collision_point.x < 0 ||
       collision_point.x > collision(upper_cushion_, lower_cushion_).x)) {
    b = Ball{0., 4.};
  } else {
    b = Ball{ball_path.slope * lenght_ + ball_path.y_intercept,
             std::atan(ball_path.slope)};
  }
}

Sample Biliard::split(std::vector<Ball> &balls) {
  std::vector<Ball> balls_sample;
  std::for_each(balls.begin(), balls.end(), [&, this](Ball &ball) {
    this->Dynamic(ball);
    if (ball.angle != 4.) {
      balls_sample.push_back(ball);
    }
  });
  std::vector<double> angles_sample;
  angles_sample.reserve(balls_sample.size());
  std::vector<double> y_coords_sample;
  y_coords_sample.reserve(balls_sample.size());
  std::for_each(balls_sample.begin(), balls_sample.end(),
                [&](Ball const &ball) {
                  angles_sample.push_back(ball.angle);
                  y_coords_sample.push_back(ball.y_coord);
                });
  return Sample{angles_sample, y_coords_sample};
}

}  // namespace bl
