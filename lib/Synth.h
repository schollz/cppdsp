#pragma once

class Synth {
protected:
    bool doneAction = false;
    
public:
    virtual ~Synth() = default;
    virtual void process(float** out, int blockSize) = 0;
    bool isDone() const;
};