This directory contains the following files. If the file has already
been included in CMSSW, its CMSSW location is listed as well.

* AbsElectronicODERHS.h
* CalibCalorimetry/HcalAlgos/interface/AbsElectronicODERHS.h
Specializes "AbsODERHS" class for modeling simple electronic circuits.
Introduces the "input pulse" (just one) and specifies the interface
for circuit parameters.

* AbsODERHS.h
* CalibCalorimetry/HcalAlgos/interface/AbsODERHS.h
Base class for modeling right hand sides of ordinary differential
equations (ODEs) in the standard form. See, for example, slide 9 in
Reference [1].

* CmdLine.hh
* CondTools/Hcal/interface/CmdLine.h (class moved into namespace "cmdline")
A facility for parsing command line arguments in C++. Provides functionality
similar to that of "getopt" and "getopt_long", with a convenient interface.

* ConstantStepOdeSolver.h
* CalibCalorimetry/HcalAlgos/interface/ConstantStepOdeSolver.h
A constant-time-step ODE solver. Declares the base class
"ConstantStepOdeSolver" and a few concrete algorithms (Euler, 2nd
order Runge-Kutta, 4th order Runge-Kutta).

* ConstantStepOdeSolver.cc
* CalibCalorimetry/HcalAlgos/src/ConstantStepOdeSolver.cc
Implementation of classes declared in ConstantStepOdeSolver.h.

* DecayingExpFilterTiming.h
A model for the dependence of the time constant of a low-pass filter
on the QIE8 bias current: tau = a*exp(-|currentIn/b|^n) + c.  All
parameters are expected to be positive, so the parameters passed to
the model are actually logs of a, b, n, and c.

* DoublePadeDelay.h
* CalibCalorimetry/HcalAlgos/interface/DoublePadeDelay.h
The main model for the QIE8 input stage. Described in slide 6 of [3].

* HcalInterpolatedPulse.h
* CondFormats/HcalObjects/interface/HcalInterpolatedPulse.h
A specialization of "InterpolatedPulse" sufficient to represent 13 time
slices with 0.25 ns step.

* InterpolatedPulse.h
* CondFormats/HcalObjects/interface/InterpolatedPulse.h
A template for representing pulse shapes using discrete time steps, with
linear interpolation between the points and with proper interpolation
of derivatives.

* LowPassFilterTiming.h
* CalibCalorimetry/HcalAlgos/interface/LowPassFilterTiming.h
A special model for the dependence of the time constant of a low-pass
filter on the QIE8 bias current. This model is described in slide 5 of
Reference [2]. For complete detail, see the code and the comments inside
"LowPassFilterTiming.cc". This class is intended for use as a template
parameter of the "DoublePadeDelay" class.

* LowPassFilterTiming.cc
* CalibCalorimetry/HcalAlgos/src/LowPassFilterTiming.cc
Implementation of the "LowPassFilterTiming" class.

* PadeTableODE.h
* CalibCalorimetry/HcalAlgos/interface/PadeTableODE.h
Declaration of the class implementing "Pade Delay" differential equations,
eventually suitable for use as a template parameter for the "DoublePadeDelay"
class. See the description of these equations in slides 3 and 4 of [3].

* PadeTableODE.cc
* CalibCalorimetry/HcalAlgos/src/PadeTableODE.cc
Implementation of the PadeTableODE class.

* QIE8Simulator.h
* CalibCalorimetry/HcalAlgos/interface/QIE8Simulator.h
Simulator of the QIE8 chip, including both the input stage and the time
slice integrator (but not the digitizer).

* QIE8Simulator.cc
* CalibCalorimetry/HcalAlgos/src/QIE8Simulator.cc
Implementation of the QIE8Simulator class.

* ThirdOrderDelayODE.h
* CalibCalorimetry/HcalAlgos/interface/ThirdOrderDelayODE.h
This class calculates the right hand side of a special third order
differential equation described in slide 6 of [3].

* ThirdOrderDelayODE.cc
* CalibCalorimetry/HcalAlgos/src/ThirdOrderDelayODE.cc
Implementation of the "ThirdOrderDelayODE" class.

* ConstantFilterTiming.h
Model of a constant tau parameter, not dependent on the preamp current.
This class is intended for use as a template parameter of the
"DoublePadeDelay" class.

* exampleSimulation.cc
An example code which simulates QIE8 response to an exponentially decaying
input pulse.

* Makefile
A file needed to compile the "exampleSimulation" executable with
"make".  In principle, if you have a recent version of g++ installed
on your machine, you should be able to just type "make" in order to
compile the code. Upon compiling the code, run the "exampleSimulation"
program without any command line arguments to see its usage
instructions. Example:

./exampleSimulation 100 100 20


References:

[1] http://highenergy.phys.ttu.edu/~igv/ComputationalPhysics/Lectures/lecture2.pdf

[2] https://indico.cern.ch/event/348527/contributions/820166/attachments/687986/944839/qie8_preamp_CMSSW.pdf

[3] https://indico.cern.ch/event/351640/contributions/1754102/attachments/695716/955257/preamp_update.pdf
