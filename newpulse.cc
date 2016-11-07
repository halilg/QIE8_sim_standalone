#include <iostream>
#include <fstream>
#include "InterpolatedPulse.h"
#include "QIE8Simulator.h"
#include "PulseModel.h"
#include "TCanvas.h"
#include "TAxis.h"
#include "TGraph.h"

typedef InterpolatedPulse<QIE8Simulator::maxlen> HPDInterpolatedPulse;
#define N_PARM 3

using namespace std;
class clsPulse {
    double _parm[N_PARM];
    double _charge;
    float pulseData[QIE8Simulator::maxlen];
    HPDInterpolatedPulse * _pulse;
    double _pulseModel ( double t, double *parm);
    double _dt;
    //double _charge=1;
    void calcPulse(double *parm);

  public:
    clsPulse(double dt, double charge);
    double value(double t, double *parm);
    //inline double operator()(const double t) const;
};

double clsPulse::value(double t, double *parm){
    bool sameparm=true;
    for (int i=0; i<N_PARM; i++){
        sameparm = sameparm && (_parm[i] == parm[i]);
        _parm[i] = parm[i];
    }
    if (!sameparm) calcPulse(parm);
    return (*_pulse)(t);
}

//double clsPulse::operator()(const double t) const {
        //return (*_pulse)(t);
//}

clsPulse::clsPulse(double dt, double charge){
    _dt=dt;
    _pulse=NULL;
    _charge=charge;
    for (int i = 0; i<N_PARM; i++) _parm[i]=-999;
}

double clsPulse::_pulseModel ( double t, double *parm) {
    
    const double temp=parm[0]*SciPMTPulseModel(parm[1], parm[2], t);
    //std::cout << t[0] << " " << parm[1] << " " << parm[2] << " " << temp << std::endl;
    return temp;
};

void clsPulse::calcPulse(double *parm){
    //std::cout << "calculating pulse " << std::endl;
    double dL;
    double sciLight[QIE8Simulator::maxlen];

    // model PMT
    sciLight[0]=0;
    sciLight[1]=0.5;
    sciLight[2]=1;
    for (unsigned int i=3; i<QIE8Simulator::maxlen; i++){
        dL=-(1/parm[0])*sciLight[i-1]*_dt;
        sciLight[i]=sciLight[i-1]+dL;
        sciLight[i]=0;
    }
    sciLight[1]=0;
    
    for (unsigned int i=0; i<QIE8Simulator::maxlen; i++){
        pulseData[i]=sciLight[i];
    }
    
    if(_pulse != NULL) delete _pulse;
    _pulse = new HPDInterpolatedPulse(0.0, (QIE8Simulator::maxlen-1)*_dt,
                                pulseData, QIE8Simulator::maxlen);
    _pulse->setIntegral(_charge);
}

int main(){
    double parm[N_PARM];
    parm[0]=17;
    //parm[1]=17;
    //parm[2]=17;
    double charge=1;
    const double dt = 0.5;
    clsPulse pulse(dt,charge);
    float timeData[QIE8Simulator::maxlen];
    float pulseData[QIE8Simulator::maxlen];
    
    
    for (unsigned int i=0; i<QIE8Simulator::maxlen; i+=1){
        timeData[i]=i*dt;
        pulseData[i]=pulse.value(i*dt, parm);
        cout << timeData[i] << " " << pulseData[i] << endl;
    }
    
    
    TCanvas *c1 = new TCanvas("c1","graph",700,500);
    TGraph *gr = new TGraph(QIE8Simulator::maxlen/10,timeData,pulseData);
    gr->Draw("AC");
    c1->Print("new_pulse.pdf");


}