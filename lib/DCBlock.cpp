#include "DCBlock.h"

DCBlock::DCBlock(double r) : R(r) {}

double DCBlock::process(double input) {
    double output = input - x1 + R * y1;
    x1 = input;
    y1 = output;
    return output;
}

void DCBlock::reset() {
    x1 = 0.0;
    y1 = 0.0;
}