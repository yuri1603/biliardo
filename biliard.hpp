#ifndef BILIARD_HPP
#define BILIARD_HPP

#include <vector>

namespace bl {

struct Point {
  double x{0.};
  double y{0.};
};

struct Ball {
  double y{0.};
  double angle{0.};
};

struct Line {
  double slope_{0.};
  double y_intercept_{0.};
  Line(double const slope, double const y_intercept);
  Line(Point const& p1, Point const& p2);
  Line(Point const& p, double const slope);
};

struct Samples {
  std::vector<double> ball_ys{};
  std::vector<double> ball_angles{};
};

Point intersect(Line const& l1, Line const& l2);

Point collide_first(Line const& l1, Line const& l2, Line const& l3);

Line bounce(Line const& l1, Line const& l2);

std::vector<Ball> generate_random_balls(long unsigned int const N,
                                        double const y_mean,
                                        double const y_std_dev,
                                        double const angle_mean,
                                        double const angle_std_dev);

class Biliard {
  Line upper_cushion_;
  Line lower_cushion_;
  double lenght_;

 public:
  Biliard(double const l, double const y1, double const y2);

  inline double get_left_limit() const { return upper_cushion_.y_intercept_; }
  inline double get_right_limit() const {
    return upper_cushion_.slope_ * lenght_ + upper_cushion_.y_intercept_;
  }
  inline double get_lenght() const { return lenght_; }
  inline double get_cushion_slope() const { return upper_cushion_.slope_; }

  void compute_final_position(Ball& b) const;

  Samples split_for_stats(std::vector<Ball>& initial_balls) const;

  void trace_trajectory(Ball& ball,
                        std::vector<Point>& subsequent_points) const;
};

}  // namespace bl

#endif