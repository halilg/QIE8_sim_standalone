#ifndef grapher_h_
#define grapher_h_

#include <string>

#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TGraph.h"

#include "TAxis.h"
#include "TLegend.h"
#include <iostream>
#include <fstream>

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



#endif // grapher_h_
