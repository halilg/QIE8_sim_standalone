// Models RLC transmission line (Telegrapher's equations)
#include <iostream>

void telegrapher(const float Iin[], float Iout[],
                 const unsigned int asize, const float dt,
                 const float Vin, const float R, const float L, const float G, const float C,
                 const float dz) {

    float Vout, Vout_prev;
    float Iin_dot, Vout_dot;
    
    Iout[0]=0;
    Vout=0;
    Vout_prev=0;
    
    
    
    for (unsigned int i=1; i<asize; i++){
        Iin_dot=(Iin[i]-Iin[i-1])/dt;
        Vout = Vin- R*dz*Iin[i] - L*dz* Iin_dot;
        Vout_dot = (Vout - Vout_prev)/dt;
        Vout_prev=Vout;
        Iout[i] = Iin[i] - G*dz*Vout - C*dz*Vout_dot;
        //if (Iout[i]<0)Iout[i]=0;
        std::cout << Iout[i] << " " << Iin[i] << std::endl;
    }
    
}