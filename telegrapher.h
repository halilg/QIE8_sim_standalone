
#ifndef TELEGRAPHER_H_
#define TELEGRAPHER_H_

struct tLine // Data structure for transmission line
{
    float R; // per dz quantities
    float L;
    float G;
    float C;            
    float dz;
    float len;
};

void telegrapher(const float Iin[], float Iout[], const unsigned int asize,
                 const float dt,
                 const tLine & param);


void telegrapher_dz(const float Iin[], float Iout[], const unsigned int asize,
                 const float dt,
                 const tLine & param);

#endif // TELEGRAPHER_H_