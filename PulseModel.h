#ifndef CondFormats_HcalObjects_Pulse_Model_h_ 
#define CondFormats_HcalObjects_Pulse_Model_h_ 

void RefPulseModel(float pulseData[], const double dt, const double charge=1);
void PulseModel(float pulseData[], const double tDecay, const double dt);
void SciPMTPulseModel(float pulseData[], const double tauSci, const double tauPMT, const double dt, const double charge=1.0);
double SciPMTPulseModel(const double tauSci, const double tauPMT, const double t);

#endif // CondFormats_HcalObjects_Pulse_Model_h_ 