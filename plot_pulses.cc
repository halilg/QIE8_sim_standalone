#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TGraph.h"
#include "TLegend.h"

int main()
{
    
    TCanvas *c1 = new TCanvas("c1","multigraph",700,500);
    c1->SetGrid();
    TMultiGraph *mg = new TMultiGraph();
    TGraph *gr0 = new TGraph("ref_pulse_norm.txt");
    TGraph *gr1 = new TGraph("pulse.txt");
    TGraph *gr2 = new TGraph("pulse_preamp.txt");
    gr0->SetLineColor(kGray);
    gr1->SetLineColor(kOrange);
    gr2->SetLineColor(kBlue);
    //gr1->SetMarkerStyle(21);
    //gr1->Fit("pol6","q");
    
    mg->Add(gr0);
    mg->Add(gr1);
    mg->Add(gr2);
    
    mg->Draw("al");
    mg->GetXaxis()->SetTitle("t (ns)"); 
    mg->GetYaxis()->SetTitle("Pulse Current (Arbitrary units)");
    gPad->Modified();
    mg->GetXaxis()->SetLimits(-5.,150.);
    
    TLegend * leg = new TLegend(0.62,0.79,0.89,0.89); //coordinates are fractions of pad dimensions
    leg->AddEntry(gr2,"Preamp simulated pulse","l"); 
    leg->AddEntry(gr1,"Simulated pulse","l"); 
    leg->AddEntry(gr0,"Reference pulse","l"); 
    leg->Draw();    
    
    c1->Print("pulse.png");
    
    
    //const double TSs[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    TGraph *gr5 = new TGraph("ADC_vs_TS.txt");
    c1->Clear();

    gr5->SetTitle("");
    gr5->GetXaxis()->SetTitle("TS"); 
    gr5->GetYaxis()->SetTitle("Q");
    gr5->SetFillColor(13);
    gr5->Draw("AB");
    c1->Print("adc.png");
    
    
    return 0;
}