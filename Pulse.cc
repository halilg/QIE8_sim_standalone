#include <iostream>
#include <fstream>
#include <cmath>
#include "QIE8Simulator.h"    
#include "CmdLine.hh"

void pulse(float pulseData[], const double tDecay, const double dt){
    
    pulseData[0]=0;
    pulseData[1]=(exp(-1/tDecay)/tDecay ) / 2;  // linear increase in the first 1 ns
    
    for (unsigned i=2; i<QIE8Simulator::maxlen ; ++i)
    {
        const double t = i*dt;
        pulseData[i] = exp(-(t-1)/tDecay)/tDecay;
    }
    
}

