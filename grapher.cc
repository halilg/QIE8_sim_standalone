#include <iostream>
#include <string>

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
    std::fill(std::begin(xData), std::end(xData), 0.); // clear the memory
    std::fill(std::begin(yData), std::end(yData), 0.); // clear the memory
    lineColor=1;
    lineWidth=1;
    lineStyle=1;
}

hGraph::hGraph (const float dx, const float y[], const char * gname) {
    init();
    name = gname;
    for (unsigned int i=0; i<MAXLEN; i++){
        xData[i]=dx*i;
        yData[i]=y[i];
    }
}

hGraph::hGraph (const float x[], const float y[], const char * gname) {
    init();
    name = gname;
    for (unsigned int i=0; i<MAXLEN; i++){
        xData[i]=x[i];
        yData[i]=y[i];
    }
    fname="";
}

hGraph::hGraph (const char * fn, const char * gname) {
    init();
    name = gname;
    fname = fn;
}

hGraph::~hGraph () {
   ;
}

void hGrapher::add( const hGraph & graph){
    std::cout << "Adding: " << graph.fname.c_str() << std::endl;
    if (graph.fname == ""){
        gr = new TGraph(MAXLEN, graph.xData, graph.yData);
    }else{
        gr = new TGraph(graph.fname.c_str());
    }
    
    gr->SetLineColor(graph.lineColor);
    gr->SetMarkerColor(graph.lineColor);
    gr->SetLineWidth(graph.lineWidth);
    gr->SetLineStyle(graph.lineStyle);
    gr->SetFillColor(0);
    mg.Add(gr);
    leg.AddEntry(gr,graph.name);
}

void hGrapher::print(const char * fname){
    
    mg.Draw("al");
    mg.GetXaxis()->SetTitle(xAxisTitle.c_str()); 
    mg.GetYaxis()->SetTitle(yAxisTitle.c_str());
    gPad->Modified();
    
    if(xAxisLimits[0] !=0 && xAxisLimits[1] !=0) mg.GetXaxis()->SetLimits(xAxisLimits[0],xAxisLimits[1]);
    c1.Update();
    leg.Draw("l");
    c1.Print(fname);
}

hGrapher::hGrapher () {
    c1.SetGrid();
    //leg.SetBorderSize(0);
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