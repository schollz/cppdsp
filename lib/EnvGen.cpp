#include "EnvGen.h"
#include <cmath>

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
            return levels.back();
        }
        alpha = 0.0;
    }
    
    double startLevel = levels[currentSegment];
    double endLevel = levels[currentSegment + 1];
    double curve = curves[currentSegment];
    
    double output;
    if (curve == 0.0) {
        output = startLevel + (endLevel - startLevel) * alpha;
    } else {
        double exp_alpha = (std::exp(curve * alpha) - 1.0) / (std::exp(curve) - 1.0);
        output = startLevel + (endLevel - startLevel) * exp_alpha;
    }
    
    segmentProgress++;
    return output;
}

bool EnvGen::isDone() const { 
    return finished; 
}