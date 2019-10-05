#ifndef MATHS_H
#define MATHS_H

extern "C" float __fsin(float angle);
extern "C" float __fcos(float angle);

const double PI=3.1415926535897932384650288;

double sin(double x);

double cos(double x);

void set_fpu_cw(const unsigned short cw);

void enable_fpu();

#endif