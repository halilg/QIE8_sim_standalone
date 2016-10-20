#ifndef CalibCalorimetry_HcalAlgos_ConstantFilterTiming_h_
#define CalibCalorimetry_HcalAlgos_ConstantFilterTiming_h_

#include <cmath>
#include <cassert>

class ConstantFilterTiming
{
public:
    inline unsigned nParameters() const {return 1U;}

    inline double operator()(double /* currentIn */,
                             const double* pars, const unsigned nParams) const
    {
        assert(nParams == 1U);
        assert(pars);
        return exp(pars[0]);
    }
};

#endif // CalibCalorimetry_HcalAlgos_ConstantFilterTiming_h_
