#include <iostream>
#include <fstream>
#include <string>

#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TF1.h"
#include "TGraph.h"
#include "TLegend.h"

#include "QIE8Simulator.h"
#include "PulseModel.h"


#define MAXLEN 1500

class hGraph {
    void init();
  public:
    hGraph (const float x[], const float y[]);
    hGraph (const float x, const float y[]);
    hGraph (const char * fname);
    void dump();
   ~hGraph ();
   unsigned int lineColor;
   unsigned int lineWidth;
   std::string name;
   std::string fname;
   float xData[MAXLEN];
   float yData[MAXLEN];
   
};

void hGraph::init(){
    fname="";
    memset(xData, 0, sizeof(xData)); // clear the memory
    memset(yData, 0, sizeof(yData)); // clear the memory
}

hGraph::hGraph (const float dx, const float y[]) {
    init();
    for (unsigned int i=0; i<MAXLEN; i++){
        xData[i]=dx*i;
        yData[i]=y[i];
    }
}

hGraph::hGraph (const float x[], const float y[]) {
    init();
    for (unsigned int i=0; i<MAXLEN; i++){
        xData[i]=x[i];
        yData[i]=y[i];
    }
    fname="";
}

hGraph::hGraph (const char * fn) {
    init();
    fname = fn;
    return;
    std::ifstream myfile;
    unsigned int i=0;
    myfile.open (fname);
    while(!myfile.eof())
    {
        myfile >> xData[i] >> yData[i];
        ++i;
        if (i==MAXLEN) break; 
    }
    myfile.close();
}

hGraph::~hGraph () {
   ;
}

class hGrapher {
    TCanvas c1{"c1","Grapher",700,500};
    TMultiGraph mg{};
    TGraph * gr;
    //std::string drawopt{"ac"};
  public:
    float xAxisLimits[2];
    std::string xAxisTitle;
    std::string yAxisTitle;
    hGrapher ();
   ~hGrapher ();
    void add( const hGraph & graph);
    void print(const char * fname);
};

void hGrapher::add( const hGraph & graph){
    std::cout << "Adding: " << graph.fname.c_str() << std::endl;
    gr = new TGraph(graph.fname.c_str());
    gr->SetLineColor(graph.lineColor);
    gr->SetLineWidth(graph.lineWidth);
    mg.Add(gr);
}

void hGrapher::print(const char * fname){
    mg.Draw("ac");
    mg.GetXaxis()->SetTitle(xAxisTitle.c_str()); 
    mg.GetYaxis()->SetTitle(yAxisTitle.c_str());
    gPad->Modified();
    //if(xAxisLimits[0] !=0 && xAxisLimits[1] !=0) mg.GetXaxis()->SetLimits(xAxisLimits[0],xAxisLimits[1]);
    c1.Print(fname);
}

hGrapher::hGrapher () {
    c1.SetGrid();
    xAxisLimits[0]=0;
    xAxisLimits[1]=0;
}

hGrapher::~hGrapher () {
   ;
}


void hGraph::dump(){
    std::cout << "Data: " << name << std::endl;
    for (unsigned int i=0; i<MAXLEN; i++){
        std::cout << xData[i] << " " << yData[i] << std::endl;
    }
    
}


double myPulse ( double *t, Double_t *parm) {
    const double temp=parm[0]*SciPMTPulseModel(parm[1], parm[2], t[0]);
    //std::cout << t[0] << " " << parm[1] << " " << parm[2] << " " << temp << std::endl;
  return temp;
}

int main(){
    const double dt = 0.5;
    
    float pulseDataRef[QIE8Simulator::maxlen];
    float pulseData[QIE8Simulator::maxlen];
    memset(pulseDataRef, 0, sizeof(pulseData)); // clear the memory
    memset(pulseData, 0, sizeof(pulseData)); // clear the memory
    for (unsigned int i=0; i<MAXLEN; i++){
        pulseData[i]=0;
    }
    
    hGraph gr1("pulse.txt");
    //gr1.dump();
    hGrapher nmg;
    nmg.add(gr1);
    nmg.xAxisLimits[0]=-5;
    nmg.xAxisLimits[1]=150;
    nmg.xAxisTitle="t (ns)";
    nmg.yAxisTitle="Pulse Current (Arbitrary units)";
    nmg.print("test.pdf");
    
    return 0;
    
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

