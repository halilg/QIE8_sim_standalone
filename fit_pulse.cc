#include <iostream>
#include <string>

#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TF1.h"
#include "TGraph.h"
#include "TLegend.h"

#include "QIE8Simulator.h"
#include "PulseModel.h"

double myPulse ( double *t, Double_t *parm) {
    const double temp=parm[0]*SciPMTPulseModel(parm[1], parm[2], t[0]);
    //std::cout << t[0] << " " << parm[1] << " " << parm[2] << " " << temp << std::endl;
  return temp;
}

int main(){
    const double dt = 0.5;
    
    float pulseDataRef[QIE8Simulator::maxlen];
    float pulseData[QIE8Simulator::maxlen];
    memset(pulseDataRef, 0, QIE8Simulator::maxlen); // clear the memory
    memset(pulseData, 0, QIE8Simulator::maxlen); // clear the memory
    
    //double tauSci = 50;
    //double tauPMT = 5;
    std::string oPulse_ref("data/ref_pulse_norm.txt");
    
    TF1 *pulse_f1 = new TF1("pulse", myPulse, 0., QIE8Simulator::maxlen*dt, 3);
    pulse_f1->SetParNames ("Constant","tau_Sci","tau_PMT");
    pulse_f1->SetParameters(1, 10, 11);


    //

  
    RefPulseModel(pulseDataRef, dt);
    //SciPMTPulseModel(pulseData, tauSci, tauPMT, dt);

    
    TCanvas *c1 = new TCanvas("c1","multigraph",700,500);
    c1->SetGrid();
    TMultiGraph *mg = new TMultiGraph();

    TGraph *gr0 = new TGraph(oPulse_ref.c_str());
    gr0->SetLineColor(kOrange);
    gr0->SetLineWidth(2);

    gr0->Fit("pulse");
    
    //TGraph *gr1 = new TGraph();
    //gr1->SetLineColor(30);
    //gr1->SetLineWidth(2);
    //
    mg->Add(gr0);
    //mg->Add(gr1);
    //
    mg->Draw("ac");
    mg->GetXaxis()->SetTitle("t (ns)"); 
    mg->GetYaxis()->SetTitle("Pulse Current (Arbitrary units)");
    gPad->Modified();
    mg->GetXaxis()->SetLimits(-5.,150.);
    //mg->GetYaxis()->SetTitleOffset(1.3);
    //
    //TLegend * leg = new TLegend(0.62,0.79,0.89,0.89); //coordinates are fractions of pad dimensions
    //leg->AddEntry(gr0,"Reference pulse","l"); 
    //leg->AddEntry(gr1,"Pulse model","l");
    //
    //
    //leg->Draw();    
    //pulse_f1->Draw();
    
    c1->Print("fit_pulse.pdf");
    
}

