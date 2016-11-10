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

int main(){
    const double dt = 0.5;
    float pulse0[QIE8Simulator::maxlen];
    float pulse1[QIE8Simulator::maxlen];
 
    memset(pulse0, 0, sizeof(pulse0)); // clear the memory
    memset(pulse1, 0, sizeof(pulse1)); // clear the memory
    
    RefPulseModel(pulse0, dt);

    for (unsigned int i=0; i<QIE8Simulator::maxlen;i++){
        pulse0[i]=pulse0[i]*3.03E-5;
    }
    
    hGraph gr1(dt, pulse0);
    gr1.lineColor=kBlue;
    gr1.lineWidth=2;
    
    dump_pulse(pulse0, QIE8Simulator::maxlen, dt, "test_refpulse.txt");

    
    tLine myline;
    float Vin=100.;  // Volt
    float Vout=0.0;  // Volt
    myline.R=1.0E-3; // Ohm
    myline.L=0.5E-6; // Henry
    myline.G=0.0; //Ohm
    myline.C=1.0E-10; //Farad
    myline.len=10.0; //m
    myline.dz=myline.len/10; //m
        
    telegrapher(pulse0, pulse1, QIE8Simulator::maxlen, Vin, Vout, dt, myline);
    dump_pulse(pulse1, QIE8Simulator::maxlen, dt, "test_telpulse.txt");
    
    hGraph gr2(dt, pulse0);
    gr2.lineColor=kGreen;
    gr2.lineWidth=2;
    gr2.lineStyle=2;
    
    hGrapher nmg;
    nmg.add(gr1);
    nmg.add(gr2);
    nmg.xAxisLimits[0]=-5;
    nmg.xAxisLimits[1]=150;
    nmg.xAxisTitle="t (ns)";
    nmg.yAxisTitle="Pulse Current";
    nmg.print("test.pdf");
    
    
    
}