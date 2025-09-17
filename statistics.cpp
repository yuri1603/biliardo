#include "statistics.hpp"

#include <cmath>
#include <numeric>

namespace bl {

double evaluate_mean(std::vector<double> const& sample) {
  return (std::accumulate(sample.begin(), sample.end(), 0.)) /
         static_cast<double>(sample.size());
}

double evaluate_std_dev(std::vector<double> const& sample) {
  double mu = evaluate_mean(sample);
  double num = std::accumulate(
      sample.begin(), sample.end(), 0.,
      [=](double acc, double x) { return acc + (x - mu) * (x - mu); });
  return std::sqrt(num / static_cast<double>(sample.size() - 1));
}

double evaluate_skewness(std::vector<double> const& sample) {
  double mu = evaluate_mean(sample);
  double sigma = evaluate_std_dev(sample);
  double num = std::accumulate(sample.begin(), sample.end(), 0.,
                               [=](double acc, double x) {
                                 return acc + std::pow((x - mu) / sigma, 3);
                               });
  return num / static_cast<double>(sample.size());
}

double evaluate_kurtosis(std::vector<double> const& sample) {
  double mu = evaluate_mean(sample);
  double sigma = evaluate_std_dev(sample);
  double num = std::accumulate(sample.begin(), sample.end(), 0.,
                               [=](double acc, double x) {
                                 return acc + std::pow((x - mu) / sigma, 4);
                               });
  return num / static_cast<double>(sample.size());
}

Statistics evaluate_statistics(std::vector<double> const& sample) {
  return Statistics{evaluate_mean(sample), evaluate_std_dev(sample),
                    evaluate_skewness(sample), evaluate_kurtosis(sample)};
}

}  // namespace bl