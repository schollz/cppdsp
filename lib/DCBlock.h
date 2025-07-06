#pragma once

class DCBlock {
private:
    double x1 = 0.0;
    double y1 = 0.0;
    double R = 0.995;
    
public:
    DCBlock(double r = 0.995);
    double process(double input);
    void reset();
};