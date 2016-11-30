#ifndef CondFormats_HcalObjects_Pulse_Model_h_ 
#define CondFormats_HcalObjects_Pulse_Model_h_ 


template <class T>
inline
void normalize_array(T pulse[], unsigned int size, float N=1){
    T sum=0;
    for (unsigned int i=0; i<size; i++){
        sum+=pulse[i];
    }
    //std::cout << "sum= " << sum << std::endl;
    T C=N/sum;
    for (unsigned int i=0; i<size; i++){
        pulse[i]*=C;
    }
}

void dump_pulse(float pulse[], const unsigned int size, double dt, const char * fname);

void RefPulseModel(float pulseData[], const double dt, const double charge=1);
void PulseModel(float pulseData[], const double tDecay, const double dt);
void SciPMTPulseModel(float pulseData[], const double tauSci, const double tauPMT, const double dt, const double charge=1.0);
double SciPMTPulseModel(const double tauSci, const double tauPMT, const double t);
void SciModel(float pulseData[],
              const double tDecayF, const double tDecayM, const double tDecayS,
              const float wF, const float wM, const float wS,
              const double dt);
void HPDModel(const float pulseIn[], float pulseOut[], const double tDecay, const double dt);

#endif // CondFormats_HcalObjects_Pulse_Model_h_ 