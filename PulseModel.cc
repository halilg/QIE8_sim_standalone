#include <iostream>
#include <fstream>
#include <cmath>
#include "QIE8Simulator.h"    
#include "CmdLine.hh"

double SciPMTPulseModel(const double tauSci, const double tauPMT, const double t){
    if (tauSci == tauPMT){
        return (1/(tauPMT*tauPMT))*t*exp(-t/tauSci);
    }else{
        return -(1/(tauPMT-tauSci))*(exp(-t/tauSci)-exp(-t/tauPMT));
    }
    
}


void SciPMTPulseModel(float pulseData[], const double tauSci, const double tauPMT, const double dt, const double charge=1.0){
    
    double integral=0;
    for (unsigned i=2; i<QIE8Simulator::maxlen ; ++i)
    {
        const double t = i*dt;
        pulseData[i]=SciPMTPulseModel(tauSci, tauPMT, t);
        integral=integral+pulseData[i]*dt;
    }
    
    // Normalize to charge fC
    const double norm=charge/integral;
    for (unsigned i=2; i<QIE8Simulator::maxlen ; ++i)
    {
        pulseData[i]=norm*pulseData[i];
    }
}

void RefPulseModel(float pulseData[], const double dt, const double charge=1.0){

    if(dt!=0.50)throw "RefPulseModel work only for dt=0.50 ns";

    unsigned int i = 0;
    float temp;
    std::ifstream myfile_i;
    
    myfile_i.open ("data/ref_pulse_norm.txt");
    while(!myfile_i.eof())
    {
        myfile_i >> temp >> pulseData[i];
        
        bool is50ns=int(temp * 4) % 2 == 0;
        if (!is50ns) continue; // skip multiples of 0.25 ns
        pulseData[i]=pulseData[i]*charge; // fix the normalization
        ++i;
        if (i==QIE8Simulator::maxlen) break; 
    }
    myfile_i.close();
}

void PulseModel(float pulseData[], const double tDecay, const double dt){
    
    pulseData[0]=0;
    pulseData[1]=(exp(-1/tDecay)/tDecay ) / 2;  // linear increase in the first 1 ns
    
    for (unsigned i=2; i<QIE8Simulator::maxlen ; ++i)
    {
        const double t = i*dt;
        pulseData[i] = exp(-(t-1)/tDecay)/tDecay;
    }
    
}

