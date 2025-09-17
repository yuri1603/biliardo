#include "statistics.hpp"

#include <cmath>
#include <numeric>

namespace bl {

double compute_mean(std::vector<double> const& sample) {
  return (std::accumulate(sample.begin(), sample.end(), 0.)) /
         static_cast<double>(sample.size());
}

double compute_std_dev(std::vector<double> const& sample) {
  const double mu = compute_mean(sample);
  const double num = std::accumulate(
      sample.begin(), sample.end(), 0.,
      [=](double acc, double x) { return acc + (x - mu) * (x - mu); });
  return std::sqrt(num / static_cast<double>(sample.size() - 1));
}

double compute_skewness(std::vector<double> const& sample) {
  const double mu = compute_mean(sample);
  const double sigma = compute_std_dev(sample);
  double num = std::accumulate(sample.begin(), sample.end(), 0.,
                               [=](double acc, double x) {
                                 return acc + std::pow((x - mu) / sigma, 3);
                               });
  return num / static_cast<double>(sample.size());
}

double compute_kurtosis(std::vector<double> const& sample) {
  const double mu = compute_mean(sample);
  const double sigma = compute_std_dev(sample);
  double num = std::accumulate(sample.begin(), sample.end(), 0.,
                               [=](double acc, double x) {
                                 return acc + std::pow((x - mu) / sigma, 4);
                               });
  return num / static_cast<double>(sample.size());
}

Statistics compute_statistics(std::vector<double> const& sample) {
  return Statistics{compute_mean(sample), compute_std_dev(sample),
                    compute_skewness(sample), compute_kurtosis(sample)};
}

}  // namespace bl