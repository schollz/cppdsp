#include <algorithm>
#include <fstream>
#include <iostream>
#include <vector>

#include "../../lib/EnvGen.h"

int main() {
  // Test envelope parameters
  // std::vector<double> levels = {0.0, 1.0, 0.3, 0.0};
  // std::vector<double> times = {0.01, 1, 0.5};
  // std::vector<double> curves = {-4, 4, -4};
  std::vector<double> levels = {80, 40};
  std::vector<double> times = {0.5};
  std::vector<double> curves = {-8};
  double sampleRate = 44100.0;

  EnvGen env(levels, times, curves, sampleRate);

  // Generate envelope data
  std::vector<double> output;
  while (!env.isDone()) {
    output.push_back(env.process());
  }

  // Write to CSV file for plotting
  std::ofstream file("envelope_output.csv");
  file << "sample,value\n";
  for (size_t i = 0; i < output.size(); ++i) {
    file << i << "," << output[i] << "\n";
  }
  file.close();

  std::cout << "Envelope generated with " << output.size() << " samples"
            << std::endl;
  std::cout << "Output written to envelope_output.csv" << std::endl;
  std::cout << "Peak value: " << *std::max_element(output.begin(), output.end())
            << std::endl;

  return 0;
}