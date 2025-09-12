#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <algorithm>
#include <cmath>
#include <vector>

#include "biliard.hpp"

// #include "biliard.hpp"

namespace bl {

struct Statistics {
  double mean{0.};
  double std_dev{0.};
  double skewness{0.};
  double kurtosis{0.};
};

class Sample {
  std::vector<Ball> entries_;

 public:
  Sample(std::vector<Ball> &balls);

};

double mean(std::vector<double> const& entries_);
double std_dev(std::vector<double> const& entries_);
double skewness(std::vector<double> const& entries_);
double kurtosis(std::vector<double> const& entries_);

}  // namespace bl

#endif











