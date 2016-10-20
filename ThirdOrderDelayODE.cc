#include <cmath>
#include <cassert>
#include <stdexcept>

#include "ThirdOrderDelayODE.h"

void ThirdOrderDelayODE::calculate(const double tau, const double currentIn,
                                   double /* dIdt */, double /* d2Id2t */,
                                   const double* x, const unsigned lenX,
                                   const unsigned firstNode,
                                   double* derivative) const
{
    // Check input sanity
    if (lenX < firstNode + 3U) throw std::invalid_argument(
        "In ThirdOrderDelayODE::calculate: insufficient number of variables");
    if (tau <= 0.0) throw std::invalid_argument(
        "In ThirdOrderDelayODE::calculate: delay time is not positive");
    assert(x);
    assert(derivative);

    derivative[firstNode]   = x[firstNode+1];
    derivative[firstNode+1] = x[firstNode+2];
    derivative[firstNode+2] = 6.0/a_*(currentIn - x[firstNode] -
                                      c_*tau*x[firstNode+1] -
                                      b_/2.0*tau*tau*x[firstNode+2])/tau/tau/tau;
}

void ThirdOrderDelayODE::setParameters(const double* pars, const unsigned nPars)
{
    assert(nPars == 3U);
    assert(pars);
    a_ = exp(pars[0]);
    b_ = exp(pars[1]);
    c_ = exp(pars[2]);
}
