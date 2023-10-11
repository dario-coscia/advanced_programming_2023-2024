#include "data_processor.hpp"
#include <cmath>
#include <limits>

unsigned int DataProcessor::n_instances = 0;

DataProcessor::DataProcessor(const double *input_data,
                             const unsigned int &input_size)
    : size(input_size), data(new double[size]),
      minimum(std::numeric_limits<double>::max()),
      maximum(std::numeric_limits<double>::min()) {
  ++n_instances;

  for (unsigned int i = 0; i < size; ++i) {
    data[i] = input_data[i];

    if (data[i] < minimum) {
      minimum = data[i]; // Update the minimum if a smaller value is found.
    } else if (data[i] > maximum) {
      maximum = data[i]; // Update the maximum if a larger value is found.
    }
  }
}

DataProcessor::DataProcessor(const DataProcessor &other)
    : size(other.size), data(new double[size]), minimum(other.minimum),
      maximum(other.maximum) {
  ++n_instances;

  for (unsigned int i = 0; i < size; ++i) {
    data[i] = other[i];
  }
}

DataProcessor &DataProcessor::operator=(const DataProcessor &other) {
  if (this != &other) {
    delete[] data;
    size = other.size;
    data = new double[size];

    for (unsigned int i = 0; i < size; ++i) {
      data[i] = other[i];
    }

    minimum = other.minimum;
    maximum = other.maximum;
  }
  return *this;
}

DataProcessor DataProcessor::operator+(const DataProcessor &other) const {
  assert(size == other.size); // Check if sizes match.

  DataProcessor result(data, size); // Copy the current object.

  result.minimum = std::numeric_limits<double>::max();
  result.maximum = std::numeric_limits<double>::min();

  for (unsigned int i = 0; i < size; ++i) {
    result[i] += other[i];

    if (result[i] < result.minimum) {
      result.minimum = result[i];
    } else if (result[i] > result.maximum) {
      result.maximum = result[i];
    }
  }

  return result;
}

double DataProcessor::compute_mean() const {
  double sum = 0.0;
  for (unsigned int i = 0; i < size; ++i) {
    sum += data[i];
  }
  return sum / size;
}

double DataProcessor::compute_std_dev() const {
  const double mean = compute_mean();

  double sum_squared_diff = 0.0;
  for (unsigned int i = 0; i < size; ++i) {
    sum_squared_diff += (data[i] - mean) * (data[i] - mean);
  }

  return std::sqrt(sum_squared_diff / size);
}

std::ostream &operator<<(std::ostream &os, const DataProcessor &dp) {
  for (unsigned int i = 0; i < dp.size; ++i) {
    os << dp[i];
    if (i < dp.size - 1) {
      os << ", ";
    }
  }
  return os;
}

double compute_correlation(const DataProcessor &dp1, const DataProcessor &dp2) {
  assert(dp1.n_elements() == dp2.n_elements()); // Check if sizes match.

  const unsigned int n = dp1.n_elements();

  // Calculate the means of both datasets.
  const double mean1 = dp1.compute_mean();
  const double mean2 = dp2.compute_mean();

  // Calculate the sums of the cross-products of differences.
  double sum_cross_products = 0.0;
  double sum_diff1_squared = 0.0;
  double sum_diff2_squared = 0.0;

  for (unsigned int i = 0; i < n; ++i) {
    const double diff1 = dp1[i] - mean1;
    const double diff2 = dp2[i] - mean2;
    sum_cross_products += diff1 * diff2;
    sum_diff1_squared += diff1 * diff1;
    sum_diff2_squared += diff2 * diff2;
  }

  // Calculate the Pearson correlation coefficient.
  const double correlation =
      sum_cross_products /
      (std::sqrt(sum_diff1_squared) * std::sqrt(sum_diff2_squared));

  return correlation;
}
