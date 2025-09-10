#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <vector>

// #include "biliard.hpp"

namespace bl {

struct Statistics {
  double mean{0.};
  double std_dev{0.};
  double skewness{0.};
  double kurtosis{0.};
};

class sample {
  // std::vector<Result> entries_;
};

double mean(std::vector<double> const& entries_);
double std_dev(std::vector<double> const& entries_);
double skewness(std::vector<double> const& entries_);
double kurtosis(std::vector<double> const& entries_);

}  // namespace bl

#endif