#pragma once

class SinOsc {
private:
    double phase = 0.0;
    double sampleRate;
    
public:
    SinOsc(double sr = 44100.0);
    double process(double freq, double phaseOffset = 0.0);
};