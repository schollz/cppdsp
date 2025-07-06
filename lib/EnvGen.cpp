#include "EnvGen.h"

#include <cmath>
#include <iostream>

EnvGen::EnvGen(const std::vector<double>& lvls, const std::vector<double>& tms,
               const std::vector<double>& crvs, double sr)
    : levels(lvls), times(tms), curves(crvs), sampleRate(sr) {}

double EnvGen::process() {
  if (finished || currentSegment >= times.size()) return 0.0;

  double segmentLength = times[currentSegment] * sampleRate;
  double alpha = segmentProgress / segmentLength;

  if (alpha >= 1.0) {
    currentSegment++;
    segmentProgress = 0.0;
    if (currentSegment >= times.size()) {
      finished = true;
      std::cout << "Envelope finished\n";
      return levels.back();
    }
    alpha = 0.0;
  }

  double startLevel = levels[currentSegment];
  double endLevel = levels[currentSegment + 1];
  double curve = curves[currentSegment];

  double output;
  if (std::abs(curve) < 0.001) {
    output = startLevel + (endLevel - startLevel) * alpha;
  } else {
    double grow = std::exp(curve);
    double a = (endLevel - startLevel) / (1.0 - grow);
    double b = startLevel + a;
    output = b - (a * std::pow(grow, alpha));
  }

  segmentProgress++;
  return output;
}

bool EnvGen::isDone() const { return finished; }