#include <iostream>
#include "QIE8Simulator.h"
#include "grapher.h"
#include "PulseModel.h"

int main(){
    const double dt = 0.5;
    float pulseData[QIE8Simulator::maxlen];
    memset(pulseData, 0, sizeof(pulseData)); // clear the memory
    RefPulseModel(pulseData, dt);
}