#include "statistics.hpp"

#include <cmath>
#include <numeric>
#include <stdexcept>

namespace bl {

double mean(std::vector<double> const& sample) {
  return (std::accumulate(sample.begin(), sample.end(), 0.)) /
         static_cast<double>(sample.size());
}

double std_dev(std::vector<double> const& sample) {
  double mu = mean(sample);
  double num = std::accumulate(
      sample.begin(), sample.end(), 0.,
      [=](double acc, double x) { return acc + (x - mu) * (x - mu); });
  return std::sqrt(num / static_cast<double>(sample.size() - 1));
}

double skewness(std::vector<double> const& sample) {
  double mu = mean(sample);
  double sigma = std_dev(sample);
  double num = std::accumulate(
      sample.begin(), sample.end(), 0., [=](double acc, double x) {
        return acc + std::pow((x - mu) / sigma, 3);
      });
  return num / static_cast<double>(sample.size());
}

double kurtosis(std::vector<double> const& sample) {
  double mu = mean(sample);
  double sigma = std_dev(sample);
  double num = std::accumulate(
      sample.begin(), sample.end(), 0., [=](double acc, double x) {
        return acc + std::pow((x - mu) / sigma, 4);
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