#include "KickSynth.h"

#include <cmath>
#include <iostream>

constexpr double PI = 3.14159265358979323846;

KickSynth::KickSynth(double sr, double bf, double r, double st, double pa,
                     double a, double d1, double d1L, double d2, double c)
    : osc(sr),
      freqEnv({bf * r, bf}, {st}, {-8.0}, sr),
      ampEnv({c, 1.0, d1L, 0.0}, {0.005, d1, d2}, {-4.0, -4.0, -4.0}, sr),
      basefreq(bf),
      ratio(r),
      sweeptime(st),
      preamp(pa),
      amp(a),
      decay1(d1),
      decay1L(d1L),
      decay2(d2),
      clicky(c) {}

double KickSynth::tanh_distort(double x) { return std::tanh(x); }

void KickSynth::process(float** out, int blockSize) {
  for (int i = 0; i < blockSize; i++) {
    double freq = freqEnv.process();
    double env = ampEnv.process();
    std::cout << "freq: " << freq << ", env: " << env << std::endl;

    if (ampEnv.isDone()) {
      this->doneAction = true;
    }

    double sig = osc.process(freq, 0.5 * PI) * preamp;
    sig = tanh_distort(sig) * env;
    sig = tanh_distort(sig * amp);

    out[0][i] += sig;
    out[1][i] += sig;
  }
}
