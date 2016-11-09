#include <iostream>

#include "TAxis.h"
#include "TLegend.h"

#include "grapher.h"

/*
 Example implementation:
 
    hGraph gr1("pulse.txt");
    gr1.lineColor=kOrange;
    gr1.lineWidth=2;
    
    hGrapher nmg;
    nmg.add(gr1);
    nmg.xAxisLimits[0]=-5;
    nmg.xAxisLimits[1]=150;
    nmg.xAxisTitle="t (ns)";
    nmg.yAxisTitle="Pulse Current (Arbitrary units)";
    nmg.print("test.pdf");
*/

void hGraph::init(){
    fname="";
    memset(xData, 0, sizeof(xData)); // clear the memory
    memset(yData, 0, sizeof(yData)); // clear the memory
    lineColor=1;
    lineWidth=1;
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
}

hGraph::~hGraph () {
   ;
}

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
    if(xAxisLimits[0] !=0 && xAxisLimits[1] !=0) mg.GetXaxis()->SetLimits(xAxisLimits[0],xAxisLimits[1]);
    c1.Update();
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