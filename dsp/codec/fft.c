#include "ti/mathlib/mathlib.h"
#include "dsplib.h"


//Macros
//Software break point
#define SW_BREAKPOINT     asm(" SWBP 0 ");

#define PI                3.14159
#define F_TOL             (1e-06)

//Global Variable
// FFT Signal Input
#pragma DATA_ALIGN(CFFT_In, 8);
float CFFT_In[2*1024];

// FFT Signal Output
#pragma DATA_ALIGN(CFFT_Out, 8);
float CFFT_Out[2*1024];

//Store rotation factor
#pragma DATA_ALIGN(Cw, 8);
float Cw[2*1024];

//Mode
#pragma DATA_ALIGN(Cmo, 8);
float Cmo[1024];

//Binary bit reversal
#pragma DATA_ALIGN (brev, 8);
unsigned char brev[64]=
{
	0x0, 0x20, 0x10, 0x30, 0x8, 0x28, 0x18, 0x38,
	0x4, 0x24, 0x14, 0x34, 0xc, 0x2c, 0x1c, 0x3c,
	0x2, 0x22, 0x12, 0x32, 0xa, 0x2a, 0x1a, 0x3a,
	0x6, 0x26, 0x16, 0x36, 0xe, 0x2e, 0x1e, 0x3e,
	0x1, 0x21, 0x11, 0x31, 0x9, 0x29, 0x19, 0x39,
	0x5, 0x25, 0x15, 0x35, 0xd, 0x2d, 0x1d, 0x3d,
	0x3, 0x23, 0x13, 0x33, 0xb, 0x2b, 0x1b, 0x3b,
	0x7, 0x27, 0x17, 0x37, 0xf, 0x2f, 0x1f, 0x3f
};
//Function Declaration
   static void tw_gen(float *w, int n);
   void FFT(float *Input, float *Cmo, unsigned int Tn);


//Generate rotation factor
static void tw_gen(float *w, int n)
{
   	int i,j,k;
   	double x_t,y_t,theta1,theta2,theta3;

   	for(j=1,k=0;j<=n>>2;j=j<<2)
   	{
   		for(i=0;i<n>>2;i+=j)
   		{
   			theta1=2*PI*i/n;
   			x_t=cossp(theta1);
   			y_t=sinsp(theta1);
   			w[k]=(float)x_t;
   			w[k+1]=(float)y_t;

   			theta2=4*PI*i/n;
   			x_t=cossp(theta2);
   			y_t=sinsp(theta2);
   			w[k+2]=(float)x_t;
   			w[k+3]=(float)y_t;

   			theta3=6*PI*i/n;
   			x_t=cossp(theta3);
   			y_t=sinsp(theta3);
   			w[k+4]=(float)x_t;
   			w[k+5]=(float)y_t;
   			k+=6;
   		}
   	}
   }


// FFT Algorism
void FFT(float *Input, float *Cmo, unsigned int Tn)
{
    int i;

   	unsigned char rad;
   	if(Tn==16 || Tn==64 || Tn==256 || Tn==1024 || Tn==4096 || Tn==16384 || Tn==65536)
   		rad=4;
   	else if(Tn==8 || Tn==32 || Tn==128 || Tn==512 || Tn==2048 || Tn==8192 || Tn==32768 || Tn==131072)
   		rad=2;
   	else
   	{
   		return;
   	}

   	// Plural FFT
   	for(i=0;i<Tn;i++)
   	{
   		CFFT_In[2*i]=Input[i];		// Real part
   		CFFT_In[2*i+1]=0;     		// Imaginary part
   	}

   	// Generate rotation factor
   	tw_gen(Cw,Tn);

   	// FFT Calculation
   	DSPF_sp_fftSPxSP(Tn,CFFT_In,Cw,CFFT_Out,brev,rad,0,Tn);

   	// Calculate Amplitude
   	for(i=0;i<Tn;i++)
   	{
   		Cmo[i]=sqrtsp(CFFT_Out[2*i]*CFFT_Out[2*i]+CFFT_Out[2*i+1]*CFFT_Out[2*i+1]);

   		if(i==0)
   			Cmo[i]=Cmo[i]/Tn;
   		else
   			Cmo[i]=Cmo[i]*2/Tn;
   	}
}

