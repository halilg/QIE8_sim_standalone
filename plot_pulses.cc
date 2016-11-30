#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TAxis.h"
#include "TGraph.h"
#include "TLegend.h"
#include "string.h"

#include "CmdLine.hh"
#include "PulseModel.h"
#include "QIE8Simulator.h"
#include "grapher.h"

using namespace std;

static void print_usage(const char* progname)
{
    cout << "\nUsage: " << progname << " [<pulse> <preamp simulated pulse> <reference preamp simulated pulse>]\n\n"
         << "This program makes a comparison plot of the preamp input pulse, preamp simulated pulse and reference preamp simulated pulse\n"
         << "Command line arguments are as follows:\n\n"
         << "  <pulse>  normally pulse.txt\n\n"
         << "  <preamp simulated pulse> normally pulse_preamp.txt\n\n"
         << "  <reference preamp simulated pulse>  normally data/h_<charge>_out.txt\n"
         << endl;
}


static void plot_pulses_indv()
{
    const double dt = 0.5;
    float pulse0[QIE8Simulator::maxlen];
    //float time[QIE8Simulator::maxlen];
    
    memset(pulse0, 0, sizeof(pulse0)); // clear the memory
    //memset(time, 0, sizeof(time)); // clear the memory
    //values from https://github.com/halilg/cmssw/blob/CMSSW_8_1_X/CalibCalorimetry/HcalAlgos/src/HcalPulseShapes.cc
    double tDecayF=8.0; //ns
    double tDecayM=15.0; //ns
    double tDecayS=25.0; //ns
    float wF=2.; //ns
    float wM=0.7; //ns
    float wS=0.5; //ns

    //for (unsigned i=0; i<QIE8Simulator::maxlen; i++){
    //    time[i]=dt*i;
    //}

    SciModel(pulse0, tDecayF, tDecayM, tDecayS, wF, wM, wS, dt);
    normalize_array<float>(pulse0, QIE8Simulator::maxlen);

    hGraph gr1(dt, pulse0);
    gr1.lineColor=kBlue;
    gr1.lineWidth=2;

    hGrapher nmg;
    nmg.add(gr1);
    nmg.xAxisLimits[0]=-5;
    nmg.xAxisLimits[1]=150;
    nmg.xAxisTitle="t (ns)";
    nmg.yAxisTitle="Pulse (Arbitrary units)";
    nmg.print("pulse_sci.pdf");
    
    
    memset(pulse0, 0, sizeof(pulse0)); // clear the memory
    HPDModel(pulse0, 4.0, dt);
    normalize_array<float>(pulse0, QIE8Simulator::maxlen);

    hGraph gr2(dt, pulse0);
    gr2.lineColor=kBlue;
    gr2.lineWidth=2;

    hGrapher nmg1;
    nmg1.add(gr2);
    nmg1.xAxisLimits[0]=-5;
    nmg1.xAxisLimits[1]=150;
    nmg1.xAxisTitle="t (ns)";
    nmg1.yAxisTitle="Pulse (Arbitrary units)";
    nmg1.print("pulse_HPD.pdf");

    
}

int main(int argc, char *argv[])
{
    
    // Parse the command line arguments
    CmdLine cmdline(argc, argv);
    string iPulse, oPulse, oPulse_ref;
    if (argc == 2)
    {
        bool a=cmdline.has("-h");
        if (a) print_usage(cmdline.progname());
        return 0;
    }else if (argc == 1)
    {
        plot_pulses_indv();
        return 0;
    }

    

    try {
        cmdline.optend();

        if (cmdline.argc() != 3)
            throw CmdLineError("wrong number of command line arguments");
        cmdline >> iPulse >> oPulse >> oPulse_ref;

        //if (tDecay <= 0.0)
            //throw CmdLineError("Decay time argument must be positive");
    }
    catch (CmdLineError& e) {
        cerr << "Error in " << cmdline.progname() << ": "
             << e.str() << endl;
        print_usage(cmdline.progname());
        return 1;
    }    
    
    
    TCanvas *c1 = new TCanvas("c1","multigraph",700,500);
    c1->SetGrid();
    TMultiGraph *mg = new TMultiGraph();
    //TGraph *gr0 = new TGraph("ref_pulse_norm.txt");
    TGraph *gr0 = new TGraph(oPulse_ref.c_str());
    gr0->SetLineColor(kOrange);
    gr0->SetLineWidth(2);

    TGraph *gr1 = new TGraph(iPulse.c_str());
    gr1->SetLineColor(30);
    gr1->SetLineWidth(2);

    TGraph *gr2 = new TGraph(oPulse.c_str());
    gr2->SetLineColor(kBlue);
    gr2->SetLineWidth(2);
    
    
    //gr1->SetMarkerStyle(21);
    //gr1->Fit("pol6","q");
    
    mg->Add(gr0);
    mg->Add(gr1);
    mg->Add(gr2);
    
    mg->Draw("ac");
    mg->GetXaxis()->SetTitle("t (ns)"); 
    mg->GetYaxis()->SetTitle("Pulse Current (Arbitrary units)");
    gPad->Modified();
    mg->GetXaxis()->SetLimits(-5.,150.);
    mg->GetYaxis()->SetTitleOffset(1.3);
    
    TLegend * leg = new TLegend(0.62,0.79,0.89,0.89); //coordinates are fractions of pad dimensions
    leg->AddEntry(gr2,"Preamp simulated pulse","l");
    leg->AddEntry(gr0,"SPICE output","l"); 
    leg->AddEntry(gr1,"Input pulse","l");
    
    
    leg->Draw();    
    
    c1->Print("pulse.pdf");
    
    
    //const double TSs[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    
    TGraph *gr5 = new TGraph("ADC_vs_TS.txt");
    c1->Clear();

    gr5->SetTitle("");
    gr5->GetXaxis()->SetTitle("TS"); 
    gr5->GetYaxis()->SetTitle("Q");
    gr5->SetFillColor(13);
    gr5->Draw("AB");
    c1->Print("adc.pdf");
    
    
    return 0;
}