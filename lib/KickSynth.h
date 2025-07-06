#pragma once
#include "EnvGen.h"
#include "SinOsc.h"
#include "Synth.h"

class KickSynth : public Synth {
 public:
  enum DoneEnvelope { FREQ_ENV, AMP_ENV };

 private:
  SinOsc osc;
  EnvGen freqEnv;
  EnvGen ampEnv;
  double basefreq, ratio, sweeptime, preamp, amp, decay1, decay1L, decay2,
      clicky;
  DoneEnvelope doneEnvelope;

  double tanh_distort(double x);

 public:
  KickSynth(double sr = 44100.0, double bf = 40.0, double r = 6.0,
            double st = 0.05, double pa = 1.0, double a = 1.0, double d1 = 0.3,
            double d1L = 0.8, double d2 = 0.15, double c = 0.0,
            DoneEnvelope doneEnv = AMP_ENV);
  void process(float** out, int blockSize) override;
};