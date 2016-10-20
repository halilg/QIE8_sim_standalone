PROGRAMS = exampleSimulation.cc

OFILES = ConstantStepOdeSolver.o LowPassFilterTiming.o PadeTableODE.o \
         QIE8Simulator.o ThirdOrderDelayODE.o

OPTIMIZE = -std=c++11 -g
INCLUDES = -I.
CXXFLAGS = $(OPTIMIZE) $(INCLUDES) -fPIC -Wall -W -Werror
LINKFLAGS = $(OPTIMIZE) -fPIC

LIBS = -lm

%.o : %.cc
	g++ -c $(CXXFLAGS) -MD $< -o $@
	@sed -i 's,\($*\.o\)[:]*\(.*\),$@: $$\(wildcard\2\)\n\1:\2,g' $*.d

BINARIES = $(PROGRAMS:.cc=)

all: $(BINARIES)

$(BINARIES): % : %.o $(OFILES); g++ $(LINKFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(BINARIES) core.* *.o *.d *~

-include $(OFILES:.o=.d)
-include $(PROGRAMS:.cc=.d)
