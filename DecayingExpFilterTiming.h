#ifndef CalibCalorimetry_HcalAlgos_DecayingExpFilterTiming_h_
#define CalibCalorimetry_HcalAlgos_DecayingExpFilterTiming_h_

#include <cmath>
#include <cassert>

class DecayingExpFilterTiming
{
public:
    inline unsigned nParameters() const {return 4U;}

    inline double operator()(const double currentIn,
                             const double* pars, const unsigned nParams) const
    {
        // Implement tau = a*exp(-|currentIn/b|^n) + c
        // All parameters are expected to be positive, so we use logs
        // to fit them instead of actual values.
        assert(nParams == 4U);
        assert(pars);

        const double a = exp(pars[0]);
        const double b = exp(pars[1]);
        const double n = exp(pars[2]);
        const double c = exp(pars[3]);

        return a*exp(-pow(fabs(currentIn/b), n)) + c;
    }
};

#endif // CalibCalorimetry_HcalAlgos_DecayingExpFilterTiming_h_
