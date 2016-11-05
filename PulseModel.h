#ifndef CondFormats_HcalObjects_Pulse_Model_h_ 
#define CondFormats_HcalObjects_Pulse_Model_h_ 

void RefPulseModel(float pulseData[], const double dt, const double charge=1);
void PulseModel(float pulseData[], const double tDecay, const double dt);

#endif // CondFormats_HcalObjects_Pulse_Model_h_ 