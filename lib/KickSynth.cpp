#include "KickSynth.h"

#include <cmath>
#include <iostream>

constexpr double PI = 3.14159265358979323846;

KickSynth::KickSynth(double sr, double base_frequency, double ratio,
                     double sweep_time, double preamp, double amp,
                     double decay1, double decay1_length, double decay2,
                     double decay2_length, double clickiness)
    : osc(sr),
      freqEnv({base_frequency * ratio, base_frequency}, {sweep_time}, {-8.0},
              sr),
      ampEnv({clickiness, decay1, decay2, 0.0},
             {0.05, decay1_length, decay2_length}, {-4.0, -4.0, -4.0}, sr),
      base_frequency(base_frequency),
      ratio(ratio),
      sweep_time(sweep_time),
      preamp(preamp),
      amp(amp),
      decay1(decay1),
      decay1_length(decay1_length),
      decay2(decay2),
      decay2_length(decay2_length),
      clickiness(clickiness) {
  this->sampleRate = sr;
  this->phase_random = static_cast<double>(rand()) / RAND_MAX * PI;
}

double KickSynth::tanh_distort(double x) { return std::tanh(x); }

void KickSynth::process(float** out, int blockSize) {
  for (int i = 0; i < blockSize; i++) {
    double freq = freqEnv.process();
    double env = ampEnv.process();

    if (ampEnv.isDone()) {
      this->doneAction = true;
    }

    // Handle fade-out
    if (fadingOut) {
      fadeOutCounter += 1.0 / sampleRate;
      if (fadeOutCounter >= fadeOutTime) {
        this->doneAction = true;
        fadeOutMultiplier = 0.0;
      } else {
        fadeOutMultiplier = 1.0 - (fadeOutCounter / fadeOutTime);
      }
    }

    double sig = osc.process(freq + phase_random, 0.5 * PI) * preamp;
    sig = tanh_distort(sig) * env;
    sig = tanh_distort(sig * amp);

    // Apply fade-out multiplier
    sig *= fadeOutMultiplier;

    out[0][i] += sig;
    out[1][i] += sig;
  }
}
