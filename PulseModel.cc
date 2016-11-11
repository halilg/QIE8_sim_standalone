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

// Scintillator model
void SciModel(float pulseData[],
              const double tDecayF, const double tDecayM, const double tDecayS,
              const float wF, const float wM, const float wS,
              const double dt){
    float dLF;
    float dLM;
    float dLS;
    float cF=wF;
    float cS=wS;
    float cM=wM;
    pulseData[0]=0;
    pulseData[2]=wF+wM+wS;
    pulseData[1]=pulseData[2]/2;
    
    
    
    
    for (unsigned int i=3; i<QIE8Simulator::maxlen; i++){
        dLF=-cF*(1/tDecayF)*dt;
        dLM=-cM*(1/tDecayM)*dt;
        dLS=-cS*(1/tDecayS)*dt;
        cF+=dLF;
        cM+=dLM;
        cS+=dLS;
        pulseData[i]=cF+cM+cS;
    }

}


// HPD model
void HPDModel(const float pulseIn[], float pulseOut[], const double tDecay, const double dt){
    
    const int delay=8/dt; // turn on delay
    //const int rise_time=1; // ns
    const float uA_per_light=0.6;
    //float targetI=;
    std::cout << tDecay << std::endl;
    float uA=0;
    float dI;
    for (unsigned int i=1; i<QIE8Simulator::maxlen-delay; i++){
        //targetI=pulseIn[i]*uA_per_light;
        //if (pulseOut[i+delay] < targetI) {
            //pulseOut[i+delay]=uA_per_light*pulseIn[i];
        //}else{
//            
        //}
        uA=uA_per_light*pulseIn[i];
        if (pulseOut[i+delay-1]<=uA){
            //model rise
            pulseOut[i+delay]=uA;
        }else{
            //model fall
            dI=-(1/tDecay)*(pulseOut[i+delay-1]-uA)*dt;
            pulseOut[i+delay]=pulseOut[i+delay-1]+dI;
        }
        
        
        
        
        //dL=-(1/tDecay)*pulseData[i-1]*dt;
        //pulseData[i]=pulseData[i-1]+dL;
    }

}