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
      decay2, clickiness;

  double tanh_distort(double x);

 public:
  KickSynth(double sr = 44100.0, double base_frequency = 34.0,
            double ratio = 8.0, double sweep_time = 0.1, double preamp = 1.0,
            double amp = 0.5, double decay1 = 0.4, double decay1_length = 0.1,
            double decay2 = 0.15, double clickiness = 0.0);
  void process(float** out, int blockSize) override;
};