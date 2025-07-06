#include "SinOsc.h"
#include <cmath>

constexpr double PI = 3.14159265358979323846;

SinOsc::SinOsc(double sr) : sampleRate(sr) {}

double SinOsc::process(double freq, double phaseOffset) {
    double output = std::sin(phase + phaseOffset);
    phase += 2.0 * PI * freq / sampleRate;
    if (phase >= 2.0 * PI) phase -= 2.0 * PI;
    return output;
}