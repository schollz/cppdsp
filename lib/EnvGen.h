#pragma once
#include <vector>

class EnvGen {
private:
    std::vector<double> levels;
    std::vector<double> times;
    std::vector<double> curves;
    double sampleRate;
    int currentSegment = 0;
    double segmentProgress = 0.0;
    bool finished = false;
    
public:
    EnvGen(const std::vector<double>& lvls, const std::vector<double>& tms, 
           const std::vector<double>& crvs, double sr = 44100.0);
    double process();
    bool isDone() const;
};