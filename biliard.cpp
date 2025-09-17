#include "biliard.hpp"

#include <algorithm>
#include <cmath>
#include <random>
#include <vector>

namespace bl {

Line::Line(Point const &p1, Point const &p2) : slope_{0.}, y_intercept_{p2.y} {
  if (p1.y != p2.y) {
    slope_ = (p2.y - p1.y) / (p2.x - p1.x);
    y_intercept_ = p2.y - p2.x * slope_;
  }
}

Line::Line(Point const &p1, double const slope)
    : slope_{slope}, y_intercept_{p1.y - slope * p1.x} {}

Line::Line(double slope, double y_intercept)
    : slope_{slope}, y_intercept_{y_intercept} {}

Point intersect(const Line &l1, const Line &l2) {
  if (l1.slope_ == l2.slope_) {
    return Point{-1., 0.};  // default value, work well with collide_first
  } else {
    double x = (l1.y_intercept_ - l2.y_intercept_) / (l2.slope_ - l1.slope_);
    double y = l1.slope_ * x + l1.y_intercept_;
    return Point{x, y};
  }
}

Point collide_first(Line const &l1, Line const &l2, Line const &l3) {
  Point p1 = intersect(l1, l2);
  Point p2 = intersect(l1, l3);
  if (p1.x < 0) {
    return p2;
  }
  if (p2.x < 0) {
    return p1;
  }
  return p1.x < p2.x ? p1 : p2;
}

Line bounce(Line const &l1, Line const &l2) {
  Point collision_point = intersect(l1, l2);
  double new_slope =
      (l1.slope_ * std::pow(l2.slope_, 2) - l1.slope_ + 2. * l2.slope_) /
      (1 - std::pow(l2.slope_, 2) + 2. * l2.slope_ * l1.slope_);
  return Line{collision_point, new_slope};
}

Biliard::Biliard(double l, double y1, double y2)
    : upper_cushion_({0., y1}, {l, y2}),
      lower_cushion_({0., -y1}, {l, -y2}),
      lenght_(l) {}

void Biliard::compute_final_position(Ball &ball) {
  if (std::abs(ball.y) > upper_cushion_.y_intercept_ ||
      std::abs(ball.angle) > (M_PI / 2)) {
    ball = Ball{
        0., 2.};  // default unreasonable value, working with split_for_stats
  } else {
    Line ball_path{{0., ball.y}, std::tan(ball.angle)};
    Point collision_point =
        collide_first(ball_path, lower_cushion_, upper_cushion_);
    while (collision_point.x >= 0 && collision_point.x <= lenght_) {
      if (collision_point.y > 0) {
        ball_path = bounce(ball_path, upper_cushion_);
        collision_point = intersect(ball_path, lower_cushion_);
      } else if (collision_point.y < 0) {
        ball_path = bounce(ball_path, lower_cushion_);
        collision_point = intersect(ball_path, upper_cushion_);
      }
    }
    if (upper_cushion_.slope_ < 0 &&
        (collision_point.x < 0 ||
         collision_point.x > intersect(upper_cushion_, lower_cushion_).x)) {
      ball = Ball{
          0., 2.};  // default unreasonable value, working with split_for_stats
    } else {
      ball = Ball{ball_path.slope_ * lenght_ + ball_path.y_intercept_,
                  std::atan(ball_path.slope_)};
    }
  }
}

std::vector<Ball> Biliard::generate_random_balls(long unsigned int const N,
                                                 double const y_mean,
                                                 double const y_std_dev,
                                                 double const angle_mean,
                                                 double const angle_std_dev) {
  std::random_device r;
  std::default_random_engine eng{r()};
  std::normal_distribution<double> ys_distribution{y_mean, y_std_dev};
  std::normal_distribution<double> angles_distribution{angle_mean,
                                                       angle_std_dev};
  std::vector<Ball> random_balls(N);
  std::generate(random_balls.begin(), random_balls.end(), [&]() {
    return Ball{ys_distribution(eng), angles_distribution(eng)};
  });
  return random_balls;
}

Samples Biliard::split_for_stats(std::vector<Ball> &initial_balls) {
  std::vector<Ball> final_balls{};
  final_balls.reserve(initial_balls.size());
  std::for_each(initial_balls.begin(), initial_balls.end(), [&](Ball &ball) {
    compute_final_position(ball);
    if (ball.angle !=
        2.) {  // ignoring left exit balls and balls spawned out of biliard
      final_balls.push_back(ball);
    }
  });
  std::vector<double> angles_sample;
  angles_sample.reserve(final_balls.size());
  std::vector<double> y_coords_sample;
  y_coords_sample.reserve(final_balls.size());
  std::for_each(final_balls.begin(), final_balls.end(), [&](Ball const &ball) {
    angles_sample.push_back(ball.angle);
    y_coords_sample.push_back(ball.y);
  });
  return Samples{y_coords_sample, angles_sample};
}

void Biliard::trace_trajectory(Ball &ball,
                               std::vector<Point> &subsequent_points) {
  Line ball_path{{0., ball.y}, std::tan(ball.angle)};
  subsequent_points.push_back(Point{0., ball.y});
  Point collision_point =
      collide_first(ball_path, lower_cushion_, upper_cushion_);
  while (collision_point.x <= lenght_ && collision_point.x >= 0) {
    subsequent_points.push_back(Point{collision_point.x, collision_point.y});
    if (collision_point.y > 0) {
      ball_path = bounce(ball_path, upper_cushion_);
      collision_point = intersect(ball_path, lower_cushion_);
    } else if (collision_point.y < 0) {
      ball_path = bounce(ball_path, lower_cushion_);
      collision_point = intersect(ball_path, upper_cushion_);
    }
  }
  if (upper_cushion_.slope_ < 0 &&
      (collision_point.x < 0 ||
       collision_point.x > intersect(upper_cushion_, lower_cushion_).x)) {
    ball = Ball{ball_path.y_intercept_, std::atan(ball_path.slope_)};
    subsequent_points.push_back(Point{0., ball_path.y_intercept_});
  } else {
    ball = Ball{ball_path.slope_ * lenght_ + ball_path.y_intercept_,
                std::atan(ball_path.slope_)};
    subsequent_points.push_back(
        Point{lenght_, ball_path.slope_ * lenght_ + ball_path.y_intercept_});
  }
}

}  // namespace bl
