#include <cmath>
#include "QIE8Simulator.h"    
    
void pulse(float pulseData[], float timeData[], double tDecay){    
    // Simulation time step (in ns)
    const double dt = 0.5;
    
    pulseData[0]=0;
    pulseData[1]=(exp(-1/tDecay)/tDecay ) / 2;  // linear increase in the first 1 ns
    timeData[0]=0;
    timeData[1]=dt;
    for (unsigned i=2; i<QIE8Simulator::maxlen -2 ; ++i)
    {
        const double t = i*dt;
        pulseData[i] = exp(-(t-1)/tDecay)/tDecay;
        timeData[i]=t;
    }
    
}