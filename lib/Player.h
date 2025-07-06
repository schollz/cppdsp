#pragma once
#include <vector>
#include <memory>
#include "Synth.h"
#include "DCBlock.h"

class KickSynth; // Forward declaration

class Player {
private:
    std::vector<std::unique_ptr<Synth>> activeSynths;
    DCBlock dcBlockLeft;
    DCBlock dcBlockRight;
    
public:
    void trigger(std::unique_ptr<Synth> synth);
    void triggerKick(std::unique_ptr<KickSynth> kickSynth);
    void process(float** out, int blockSize);
    int getActiveSynthCount() const;
};