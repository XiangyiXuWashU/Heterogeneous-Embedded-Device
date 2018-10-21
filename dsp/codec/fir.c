#include <string.h>

#include "ti/mathlib/mathlib.h"
#include "dsplib.h"

#pragma DATA_ALIGN(input, 8);
#pragma DATA_ALIGN(output, 8);
#pragma DATA_ALIGN(firout, 8);
#pragma DATA_ALIGN(H, 8);

#define nh 4
#define ny 1020

float input[ny+nh-1];
float H[]={1.0, 0.0, 0.0, 0.0};
float output[ny];
float firout[ny];

//Function Declaration

void FIR(float *rawdata, float *firout, float *H);


//FIR Algorism
//void DSPF_sp_fir_gen(const float * restrict x, const float * restrict h, float * restrict y, int nh, int ny)
//x	 Pointer to input array. Array must have ny + nh - 1 elements.
//h Pointer to coefficient array. Array must have nh elements given in reverse order: {h(nh - 1), ..., h(1), h(0)}. Must be double-word aligned.
//y Pointer to output array. Must have ny elements.
//nh	Number of elements in coefficient array. Must be divisible by 4 and greater than 0.
//ny	Number of elements in output array. Must be divisible by 4 and greater than 0.

void FIR(float *rawdata, float *firout, float *H)
{
    int i=0;

	memset(input, 0, (ny+nh-1) * sizeof(float));
	memset(output, 0, sizeof(output));

	for(i=0 ; i<ny ; i++)
	{
	  input[i] = rawdata[i];
	}

	DSPF_sp_fir_gen (input, H, output, nh, ny);

	for(i=0 ; i<ny ; i++)
	{
	  firout[i] = output[i];
	}

}
