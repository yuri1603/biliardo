#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <vector>

namespace bl {

struct Statistics {
  double mean{0.};
  double std_dev{0.};
  double skewness{0.};
  double kurtosis{0.};
};

double evaluate_mean(std::vector<double> const& sample);
double evaluate_std_dev(std::vector<double> const& sample);
double evaluate_skewness(std::vector<double> const& sample);
double evaluate_kurtosis(std::vector<double> const& sample);
Statistics evaluate_statistics(std::vector<double> const& sample);

}  // namespace bl

#endif
