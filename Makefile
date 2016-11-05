ifndef ROOTSYS
$(warning *** ROOT environment not set)
endif

PROGRAMS = exampleSimulation.cc Dump_Pulses.cc
PROGRAMS_ROOT = plot_pulses.cc fit_pulse.cc

OFILES = ConstantStepOdeSolver.o LowPassFilterTiming.o PadeTableODE.o \
         QIE8Simulator.o ThirdOrderDelayODE.o PulseModel.o

OPTIMIZE = -std=c++11 -g
INCLUDES = -I. -I$(ROOTSYS)/include
CXXFLAGS = $(OPTIMIZE) $(INCLUDES) -fPIC -Wall -W -Werror
LINKFLAGS = $(OPTIMIZE) -fPIC

LIBS = -lm
LROOT = `root-config --cflags --glibs`

%.o : %.cc
	g++ -c $(CXXFLAGS) -MD $< -o $@
	#@sed -i 's,\($*\.o\)[:]*\(.*\),$@: $$\(wildcard\2\)\n\1:\2,g' $*.d

BINARIES = $(PROGRAMS:.cc=)
BINARIES_ROOT = $(PROGRAMS_ROOT:.cc=)

all: $(BINARIES) $(BINARIES_ROOT)

$(BINARIES): % : %.o $(OFILES); g++ $(LINKFLAGS) -o $@ $^ $(LIBS)
$(BINARIES_ROOT): % : %.o $(OFILES); g++ $(LINKFLAGS) -o $@ $^ $(LIBS) $(LROOT)


clean:
	rm -f $(BINARIES) core.* *.o *.d *~

-include $(OFILES:.o=.d)
-include $(PROGRAMS:.cc=.d)
