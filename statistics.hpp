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

std::size_t size(std::vector<double> const& sample);
double mean(std::vector<double> const& sample);
double std_dev(std::vector<double> const& sample);
double skewness(std::vector<double> const& sample);
double kurtosis(std::vector<double> const& sample);
Statistics stats(std::vector<double> const& sample);

}  // namespace bl

#endif
