#include <iostream>
#include <fstream>
#include "QIE8Simulator.h"
#include "grapher.h"
#include "telegrapher.h"
#include "PulseModel.h"

void dump_pulse(float pulse[], const unsigned int size, double dt, const char * fname){
    std::ofstream myfile(fname);
    for (unsigned int i=0;i<size; i++){
        myfile << i*dt << " " << pulse[i] << std::endl;
    }
}

template <class T>
void normalize_array(T pulse[], unsigned int size, float N=1){
    T sum=0;
    for (unsigned int i=0; i<size; i++){
        sum+=pulse[i];
    }
    std::cout << "sum= " << sum << std::endl;
    T C=N/sum;
    for (unsigned int i=0; i<size; i++){
        pulse[i]*=C;
    }
}

int main(){
    const double dt = 0.5;
    float pulse0[QIE8Simulator::maxlen];
    float pulse1[QIE8Simulator::maxlen];
 
    memset(pulse0, 0, sizeof(pulse0)); // clear the memory
    memset(pulse1, 0, sizeof(pulse1)); // clear the memory
    
    double tDecayF=2.5; //ns
    double tDecayS=20; //ns
    SciModel(pulse0, tDecayF, tDecayS, 0.5, dt);
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