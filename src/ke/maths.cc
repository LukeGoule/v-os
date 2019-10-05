#include <maths.h>
#include <system.h>

double sin(double x){
  double sign=1;
  if (x<0){
    sign=-1.0;
    x=-x;
  }
  if (x>360) x -= int(x/360)*360;
  x*=PI/180.0;
  double res=0;
  double term=x;
  int k=1;
  while (res+term!=res){
    res+=term;
    k+=2;
    term*=-x*x/k/(k-1);
  }

  return sign*res;
}

double cos(double x){
  if (x<0) x=-x;
  if (x>360) x -= int(x/360)*360;
  x*=PI/180.0;
  double res=0;
  double term=1;
  int k=0;
  while (res+term!=res){
    res+=term;
    k+=2;
    term*=-x*x/k/(k-1);
  }  
  return res;
}

/* kernel level shit */

// set the FPU's control word
void set_fpu_cw(const uint16_t cw) {
	asm volatile("fldcw %0" :: "m"(cw));
  //__setfpucw(cw);
}

// enable the FPU, it would be wiser to check if the FPU exists, 
// but we assume it does as we're targeting modern CPUs
void enable_fpu() {
  	uint32_t cr4;
  	asm volatile ("mov %%cr4, %0" : "=r"(cr4));
  	cr4 |= 0x200;
  	asm volatile ("mov %0, %%cr4" :: "r"(cr4));
  	set_fpu_cw(0x37F);
}