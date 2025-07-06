#pragma once
#include "EnvGen.h"
#include "SinOsc.h"
#include "Synth.h"

class KickSynth : public Synth {
 public:
 private:
  SinOsc osc;
  EnvGen freqEnv;
  EnvGen ampEnv;
  double base_frequency, ratio, sweep_time, preamp, amp, decay1, decay1_length,
      decay2, decay2_length, clickiness, phase_random;

  double tanh_distort(double x);

 public:
  KickSynth(double sr = 44100.0, double base_frequency = 47.0,
            double ratio = 8.0, double sweep_time = 0.1, double preamp = 2.0,
            double amp = 0.9, double decay1 = 0.4, double decay1_length = 0.05,
            double decay2 = 0.15, double decay2_length = 0.8,
            double clickiness = 0.005);
  void process(float** out, int blockSize) override;
};