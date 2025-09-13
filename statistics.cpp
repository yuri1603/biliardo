#include "statistics.hpp"

#include <cmath>
#include <numeric>
#include <stdexcept>

namespace bl {

std::size_t size(std::vector<double> const& sample) { return sample.size(); }

double mean(std::vector<double> const& sample) {
  return (std::accumulate(sample.begin(), sample.end(), 0.)) /
         static_cast<double>(size(sample));
}

double std_dev(std::vector<double> const& sample) {
  std::size_t den = size(sample) - 1;
  double num = std::accumulate(
      sample.begin(), sample.end(), 0., [&](double acc, double x) {
        return acc + (x - mean(sample)) * (x - mean(sample));
      });
  return std::sqrt(num / static_cast<double>(den));
}

double skewness(std::vector<double> const& sample) {
  double num = std::accumulate(
      sample.begin(), sample.end(), 0., [&](double acc, double x) {
        return acc + std::pow((x - mean(sample)) / std_dev(sample), 3);
      });
  return num / static_cast<double>(sample.size());
}

double kurtosis(std::vector<double> const& sample) {
  double num = std::accumulate(
      sample.begin(), sample.end(), 0., [&](double acc, double x) {
        return acc + std::pow((x - mean(sample)) / std_dev(sample), 4);
      });
  return num / static_cast<double>(sample.size());
}

Statistics stats(std::vector<double> const& sample) {
  if (sample.size() < 2) {
    throw std::domain_error(
        "Unable to run a statistic on a sample size less than 2");
  } else {
    return Statistics{mean(sample), std_dev(sample), skewness(sample),
                      kurtosis(sample)};
  }
}

}  // namespace bl