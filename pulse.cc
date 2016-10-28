#include <iostream>
#include <fstream>
#include <cmath>
#include "QIE8Simulator.h"    
#include "CmdLine.hh"

void pulse(float pulseData[], float timeData[], double tDecay){    
    // Simulation time step (in ns)
    const double dt = 0.5;
    
    pulseData[0]=0;
    pulseData[1]=(exp(-1/tDecay)/tDecay ) / 2;  // linear increase in the first 1 ns
    timeData[0]=0;
    timeData[1]=dt;
    for (unsigned i=2; i<QIE8Simulator::maxlen -2 ; ++i)
    {
        const double t = i*dt;
        pulseData[i] = exp(-(t-1)/tDecay)/tDecay;
        timeData[i]=t;
    }
    
}


using namespace std;


static void print_usage(const char* progname)
{
    cout << "\nUsage: " << progname << " tDecay [of]\n\n"
         << "This program generates the exponential input pulse \n"
         << "Command line arguments are as follows:\n\n"
         << "  tDecay   is the decay time of the exponential in ns\n"
         << "  of       is the text file pulse data will be written to. Default is pulse.txt\n"
         << endl;
}


int main(int argc, char *argv[]){
    double tDecay = 100;
    char ofname[50] = "pulse.txt";
    float pulseData[QIE8Simulator::maxlen];
    float timeData[QIE8Simulator::maxlen];
    
        // Parse the command line arguments
    CmdLine cmdline(argc, argv);

    if (argc == 1)
    {
        print_usage(cmdline.progname());
        return 0;
    }

    try {
        cmdline.optend();

        if (cmdline.argc() == 1 || cmdline.argc() > 2)
            throw CmdLineError("wrong number of command line arguments");
        if (cmdline.argc() == 1) cmdline >> tDecay;
        else if (cmdline.argc() == 2) cmdline >> tDecay >> ofname;

        if (tDecay <= 0.0)
            throw CmdLineError("Decay time argument must be positive");
    }
    catch (CmdLineError& e) {
        cerr << "Error in " << cmdline.progname() << ": "
             << e.str() << endl;
        print_usage(cmdline.progname());
        return 1;
    }
    
    
    
    pulse(pulseData, timeData,tDecay); // get the pulse
    ofstream myfile;
    myfile.open (ofname);
    cout << "tDecay=" << tDecay << endl;
    for (unsigned int i=0; i<QIE8Simulator::maxlen; i++){
        myfile << timeData[i] << " " << pulseData[i] << std::endl;
    }
    myfile.close();
    cout << "Wrote " << ofname << endl;
}