#include <iostream>
#include <fstream>
#include <iomanip>
#include <cassert>
#include <algorithm>
#include <numeric>
#include <cstring>


#include "CmdLine.hh"

#include "QIE8Simulator.h"
#include "LowPassFilterTiming.h"
#include "ConstantFilterTiming.h"
#include "DecayingExpFilterTiming.h"
#include "DoublePadeDelay.h"
#include "PadeTableODE.h"
#include "ThirdOrderDelayODE.h"
#include "PulseModel.h"

#define N_TIME_SLICES 10

// Define or undefine the following.
// It looks like defining it leads to a better coincidence
// between this simulation and SPICE.
#define USE_DECAYING_EXP_FOR_CONTROL


#ifdef USE_DECAYING_EXP_FOR_CONTROL
typedef DoublePadeDelay<ThirdOrderDelayODE,PadeTableODE,
                        LowPassFilterTiming,DecayingExpFilterTiming> MyPreamp;
#else
typedef DoublePadeDelay<ThirdOrderDelayODE,PadeTableODE,
                        LowPassFilterTiming,ConstantFilterTiming> MyPreamp;
#endif


using namespace std;

static void print_usage(const char* progname)
{
    cout << "\nUsage: " << progname << " charge tStart tDecay\n\n"
         << "This program runs one exponential input pulse through the QIE8 chip\n"
         << "simulation and prints obtained ADC counts to the standard output.\n"
         << "Command line arguments are as follows:\n\n"
         << "  charge   is the input pulse charge in fC (eg 20 fC).\n\n"
         << "  tStart   is the time at which the pulse starts in ns (eg 100 ns)\n\n"
         << "  tDecay   is the decay time of the exponential in ns (eg 40 ns, 0 uses reference input pulse)\n"
         << endl;
}

int main(int argc, char *argv[])
{

    // Simulation time step (in ns)
    const double dt = 0.5;

    // Simulation stop time (in ns)
    const double tstop = QIE8Simulator::maxlen*dt;

    // Parameters of the preamp model. In principle, they can be
    // adjusted, but this has to be done in a coherent manner.
#ifdef USE_DECAYING_EXP_FOR_CONTROL
    const double logVb_0 = 0.05553521207803361;
    const double logTau_0 = -0.08685779706819352;
    const double logV0_0 = 0.4264983917824756;
    const double logDel_0 = 1.2120910567832441;
    const double slopLg_0 = 2.548372272312645;
    const double tauMax_0 = 19.043856315437598;
    const double logA = 0.02605907291143367;
    const double logB = 0.11982864085338375;
    const double logC = 0.3249974830770151;
    const double logACtl = -0.49794727860939997;
    const double logBCtl = 2.45971197083042;
    const double logNCtl = 1.9333914027814716;
    const double logTau_1 = 0.21265500012075858;
    const double ctlGainOut = 2.8138314116133727;
    const double inGainOut = -1.3059242471301633;
    const double outGainOut = 0.3205248012375926;

    const double preampParameters[] = {logVb_0, logTau_0, logV0_0,
                                       logDel_0, slopLg_0, tauMax_0,
                                       logA, logB, logC, logACtl,
                                       logBCtl, logNCtl, logTau_1,
                                       ctlGainOut, inGainOut, outGainOut};
#else
    const double logVb_0 = 0.26971458961942774;
    const double logTau_0 = 0.4076929930011986;
    const double logV0_0 = 1.2888597202750967;
    const double logDel_0 = 0.5635590410149378;
    const double slopLg_0 = 2.8597768312847744;
    const double tauMax_0 = 29.432945200676308;
    const double logA = -1.0044946683465878;
    const double logB = -0.7131712631118886;
    const double logC = -0.0900473080097367;
    const double logTau_1 = 0.7702482974083039;
    const double ctlGainOut = 1.5897923253731723;
    const double inGainOut = -0.5275975417470286;
    const double outGainOut = 0.230625139585399;

    const double preampParameters[] = {logVb_0, logTau_0, logV0_0,
                                       logDel_0, slopLg_0, tauMax_0,
                                       logA, logB, logC, logTau_1,
                                       ctlGainOut, inGainOut, outGainOut};
#endif

    
    
    // Parse the command line arguments
    CmdLine cmdline(argc, argv);

    if (argc == 1)
    {
        print_usage(cmdline.progname());
        return 0;
    }

    double charge, tStart, tDecay;

    try {
        cmdline.optend();

        if (cmdline.argc() != 3)
            throw CmdLineError("wrong number of command line arguments");
        cmdline >> charge >> tStart >> tDecay;

        //if (tDecay <= 0.0)
            //throw CmdLineError("Decay time argument must be positive");
    }
    catch (CmdLineError& e) {
        cerr << "Error in " << cmdline.progname() << ": "
             << e.str() << endl;
        print_usage(cmdline.progname());
        return 1;
    }

    // Create the QIE chip simulator. There are some hardwired
    // parameters here which specify the kind of differential
    // equations we are going to solve.
    QIE8Simulator sim(MyPreamp(0, 3, 1, 1), 0);

    // Make sure that we are providing the correct
    // number of simulation parameters
    const unsigned np = sizeof(preampParameters)/sizeof(preampParameters[0]);
    assert(np == sim.nParameters());

    // Set simulator parameters
    sim.setLeadingParameters(preampParameters, np);
    
    
    string ofname;
    float pulseData[QIE8Simulator::maxlen];
    memset(pulseData, 0, QIE8Simulator::maxlen); // clear the memory
    double integral=0;

    if(tDecay == 0){
        // Use the reference pulse for simulation
        // read pulse data
        RefPulseModel(pulseData, dt, charge);
        cout << "Read input pulse shape from " << "ref_pulse_norm.txt" << endl;        
    }else{
        // Use pulse model
        PulseModel(pulseData, tDecay, dt);
    }
    
    // write input pulse model
    ofstream myfile;
    ofname = "pulse.txt";
    myfile.open (ofname);
    for (unsigned int i=0; i<QIE8Simulator::maxlen; i++){
        myfile << i*dt << " " << pulseData[i] << std::endl;
    }
    myfile.close();
    cout << "Wrote input pulse model: " << ofname << endl;
    
    
    // Corresponding object which knows how to interpolate
    // pulse shapes and which can be used by the simulator
    HcalInterpolatedPulse pulse(0.0, (QIE8Simulator::maxlen-1)*dt,
                                pulseData, QIE8Simulator::maxlen);
    pulse.setIntegral(charge);

    // Shift the pulse to the requested start time
    pulse.setStartTime(tStart);

    // Give the pulse to the simulator
    sim.setInputSignal(pulse);

    // Run the simulation
    double ts[N_TIME_SLICES];
    const unsigned nTS = sim.run(dt, tstop, 0.0, ts, N_TIME_SLICES);
    assert(nTS == N_TIME_SLICES);

    //dump output pulse
    ofname = "pulse_preamp.txt";
    ofstream preout;
    preout.open("pulse_preamp.txt");
    
    integral=0;
    for (unsigned int i=0; i<QIE8Simulator::maxlen; i++){
        const double time=i*dt;
        preout << time << " " << sim.preampOutput(time) << std::endl;
        integral=integral+sim.preampOutput(time)*dt;
    }
    
    preout.close();
    cout << "Wrote preamp simulated pulse: " << ofname << endl;
    cout << "integral=" << integral << endl;
    
    // Dump obtained ADC counts
    ofname = "ADC_vs_TS.txt";
    ofstream tsout;
    tsout.open (ofname);
    const double q = std::accumulate(ts+3, ts+8, 0.0);
    cout << endl << "TS3:7 Q = " << q << ", log10(Q) = " << log10(q) << '\n' << endl;
    
    for (unsigned i=0; i<N_TIME_SLICES; ++i)
    {
        cout << "TS" << i << ": " << setw(12) << ts[i]
             << setw(13) << ts[i]/q << endl;
        tsout << i << " " << setw(12) << ts[i] << " " << setw(13) << ts[i]/q << endl;
    }
    tsout.close();
    cout << "Wrote ADC values simulated pulse: " << ofname << endl;
    
    // We are done
    return 0;
}
