#include <cmath>
#include "QIE8Simulator.h"    
    
void I_in(float pulseData[], float timeData[], double tDecay){    
    // Simulation time step (in ns)
    const double dt = 0.5;
    
    for (unsigned i=0; i<QIE8Simulator::maxlen; ++i)
    {
        const double t = i*dt;
        pulseData[i] = exp(-t/tDecay)/tDecay;
        timeData[i]=t;
    }
    
}