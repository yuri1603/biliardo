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
  Statistics stats();
};

std::size_t size(std::vector<double> const&);
double mean(std::vector<double> const&);
double std_dev(std::vector<double> const&);
double skewness(std::vector<double> const&);
double kurtosis(std::vector<double> const&);

}  // namespace bl

#endif
