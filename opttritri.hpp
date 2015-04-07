#ifndef OPTTRITRI_HPP
#define OPTTRITRI_HPP

#include <math.h>

int coplanar_tri_tri(double N[3],double V0[3],double V1[3],double V2[3],
                       double U0[3],double U1[3],double U2[3]);

int NoDivTriTriIsect(double V0[3],double V1[3],double V2[3],
                     double U0[3],double U1[3],double U2[3]);

#endif
