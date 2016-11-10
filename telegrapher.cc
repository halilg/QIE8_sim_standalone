// Models RLC transmission line (Telegrapher's equations)
#include <iostream>
#include "telegrapher.h"




void telegrapher_dz(const float Iin[], float Iout[], const unsigned int asize,
                 float & Vout,
                 const float dt,
                 const tLine & param) {

    float Vout_prev;
    float Iin_dot, Vout_dot;
    const float R=6.0E6;
    Iout[0]=0;
    Vout_prev=0;
    float Vin;
    
    for (unsigned int i=1; i<asize; i++){
        Iin_dot = (Iin[i]-Iin[i-1])/dt;
        Vin= Iin[i] * R;
        Vout = Vin - param.R * param.dz * Iin[i] - param.L * param.dz * Iin_dot;
        Vout_dot = (Vout - Vout_prev) / dt;
        std::cout << "Iindot=" << Iin_dot << ", Vout=" << Vout << ", Voutdot=" << Vout_dot << std::endl;
        Vout_prev = Vout;
        Iout[i] = Iin[i] - param.G * param.dz * Vout - param.C * param.dz * Vout_dot;
        if (Iout[i]<0)Iout[i]=0;
        //std::cout << Iout[i] - Iin[i] << "I - V" << Vout - Vin << std::endl;
    }
    
}

void telegrapher(const float Iin[], float Iout[], const unsigned int asize,
                 float & Vout,
                 const float dt,
                 const tLine & param) {

    const unsigned int ndz=int(param.len/param.dz);
    float _Vout=Vout;
    float pulse[asize];

    unsigned int i,j;
    
    for (i=0; i<asize; i++){
        pulse[i]=Iin[i];
    }    
    
    for (i=0; i<ndz; i++){
        telegrapher_dz(pulse, Iout, asize, _Vout, dt, param);
        for (j=0; j<asize; j++){
            pulse[j]=Iout[j];
        }
    }
}
