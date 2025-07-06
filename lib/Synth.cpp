#include "Synth.h"

bool Synth::isDone() const { 
    return doneAction; 
}

void Synth::fadeOutAndFree(double fadeTimeSeconds) {
    if (!fadingOut) {
        fadingOut = true;
        fadeOutTime = fadeTimeSeconds;
        fadeOutCounter = 0.0;
        fadeOutMultiplier = 1.0;
    }
}