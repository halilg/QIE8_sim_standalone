// Models RLC transmission line (Telegrapher's equations)
#include <iostream>
#include "telegrapher.h"




void telegrapher_dz(const float Iin[], float Iout[], const unsigned int asize,
                 const float Vin, float & Vout,
                 const float dt,
                 const tLine & param) {

    float Vout_prev;
    float Iin_dot, Vout_dot;
    
    Iout[0]=0;
    Vout_prev=0;
    
    for (unsigned int i=1; i<asize; i++){
        Iin_dot = (Iin[i]-Iin[i-1])/dt;
        Vout = Vin - param.R * param.dz * Iin[i] - param.L * param.dz * Iin_dot;
        Vout_dot = (Vout - Vout_prev) / dt;
        Vout_prev = Vout;
        Iout[i] = Iin[i] - param.G * param.dz * Vout - param.C * param.dz * Vout_dot;
        if (Iout[i]<0)Iout[i]=0;
        //std::cout << Iout[i] - Iin[i] << "I - V" << Vout - Vin << std::endl;
    }
    
}

void telegrapher(const float Iin[], float Iout[], const unsigned int asize,
                 const float Vin, float & Vout,
                 const float dt,
                 const tLine & param) {

    const unsigned int ndz=int(param.len/param.dz);
    float _Vin=Vin;
    float _Vout=Vout;
    float pulse[asize];

    unsigned int i,j;
    
    for (i=0; i<asize; i++){
            pulse[i]=Iin[i];
    }    
    
    for (i=0; i<ndz; i++){
        telegrapher_dz(pulse, Iout, asize, _Vin, _Vout, dt, param);
        _Vin=_Vout;
        for (j=0; j<asize; j++){
            pulse[j]=Iout[j];
        }
    }
}
