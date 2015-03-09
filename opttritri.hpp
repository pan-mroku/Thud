#ifndef OPTTRITRI_HPP
#define OPTTRITRI_HPP

#include <math.h>

int coplanar_tri_tri(float N[3],float V0[3],float V1[3],float V2[3],
                       float U0[3],float U1[3],float U2[3]);

int NoDivTriTriIsect(float V0[3],float V1[3],float V2[3],
                     float U0[3],float U1[3],float U2[3]);

#endif
