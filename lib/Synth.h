#pragma once

class Synth {
protected:
    bool doneAction = false;
    bool fadingOut = false;
    double fadeOutTime = 0.005; // 5ms default
    double fadeOutCounter = 0.0;
    double fadeOutMultiplier = 1.0;
    double sampleRate = 44100.0;
    
public:
    virtual ~Synth() = default;
    virtual void process(float** out, int blockSize) = 0;
    bool isDone() const;
    void fadeOutAndFree(double fadeTimeSeconds = 0.005);
};