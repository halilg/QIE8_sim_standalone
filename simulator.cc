#include <iostream>
#include <fstream>
#include "QIE8Simulator.h"
#include "grapher.h"
#include "telegrapher.h"
#include "PulseModel.h"

int main(){
    const double dt = 0.5;
    float pulse0[QIE8Simulator::maxlen];
    float pulse1[QIE8Simulator::maxlen];
 
    std::fill(std::begin(pulse0), std::end(pulse0), 0.); // clear the memory
    std::fill(std::begin(pulse1), std::end(pulse1), 0.); // clear the memory

    //void SciModel(float pulseData[],
              //const double tDecayF, const double tDecayM, const double tDecayS,
              //const float wF, const float wM, const float wS,
              //const double dt);

    //values from https://github.com/halilg/cmssw/blob/CMSSW_8_1_X/CalibCalorimetry/HcalAlgos/src/HcalPulseShapes.cc
    double tDecayF=8.0; //ns
    double tDecayM=15.0; //ns
    double tDecayS=25.0; //ns
    float wF=2.; //ns
    float wM=0.7; //ns
    float wS=0.5; //ns

    SciModel(pulse0, tDecayF, tDecayM, tDecayS, wF, wM, wS, dt);
    normalize_array<float>(pulse0, QIE8Simulator::maxlen);
    
    //for (unsigned int i=0; i<QIE8Simulator::maxlen;i++){ // set max signal to 1 uA
        //pulse0[i]=pulse0[i]*3.03E-5;
    //}
    
    hGraph gr1(dt, pulse0);
    gr1.lineColor=kBlue;
    gr1.lineWidth=2;
    
    dump_pulse(pulse0, QIE8Simulator::maxlen, dt, "pulse_scintillator.txt");
    std::cout << "wrote pulse_scintillator.txt\n";
    
    
    //tLine myline;
    //float Vout=0.0;  // Volt
    //myline.R=1.0E-3; // Ohm
    //myline.L=0.5E-6; // Henry
    //myline.G=0.0; //Ohm
    //myline.C=1.0E-10; //Farad
    //myline.len=1.0; //m
    //myline.dz=myline.len; //m
        
    // not yet
    //telegrapher(pulse0, pulse1, QIE8Simulator::maxlen, Vout, dt*1E-9, myline);
    double tDecay=14.8; //ns
    HPDModel(pulse0, pulse1, tDecay, dt);
    normalize_array<float>(pulse1, QIE8Simulator::maxlen);
    dump_pulse(pulse1, QIE8Simulator::maxlen, dt, "pulse_hpd.txt");
    std::cout << "wrote pulse_hpd.txt\n";
    
    hGraph gr2(dt, pulse1);
    gr2.lineColor=kRed;
    gr2.lineWidth=2;
    //gr2.lineStyle=2;
    
    hGrapher nmg;
    nmg.add(gr1);
    nmg.add(gr2);
    nmg.xAxisLimits[0]=-5;
    nmg.xAxisLimits[1]=150;
    nmg.xAxisTitle="t (ns)";
    nmg.yAxisTitle="Pulse (Arbitrary units)";
    nmg.print("pulse.pdf");
}