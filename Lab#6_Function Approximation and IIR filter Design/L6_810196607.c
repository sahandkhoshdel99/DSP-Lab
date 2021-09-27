#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define UNITQ15 0x7fff
#define INPUT_FIXED_POINT_BITS 14
#define OUTPUT_FIXED_POINT_BITS 15
#define SIGNAL_LEN 10000 
#define FILTER_ORDER 4

//function decalration:


float fcos1(float x);
float fcos2(float x);
short icos(short x);

short float_to_fixed(float input, int shiftamount);
float fixed_to_float(short input, int shiftamount);

void IIR_fixed(short *coeffs_B, short *coeffs_A, short *input, short *output, int signal_length, int filterLength);
void random_signal(float* arr);


//declariing global averiables:

float fcoef[4]={1.0,-1/2.0,1.0/(2.0*3.0*4.0),
-1.0/(2.0*3.0*4.0*5.0*6.0)};

short iCoef[4]={(short)(UNITQ15),(short)(-(UNITQ15/2.0)),
(short)(UNITQ15/(2.0*3.0*4.0)),
(short)(-(UNITQ15/(2.0*3.0*4.0*5.0*6.0)))};



float SOS_[2][6] = { { 1,2,1,1, -0.00000000000000016058961851103, 0.446462692171690}, { 1,2,1,1, -0.000000000000000115415025303165, 0.039566129896580}};
float G_[3] = {0.361615673042922, 0.259891532474145, 1 };

float b = {0.0941, 0.3759, 0.5639, 0.3759, 0.0940};
float a = { 1.0, 0.0, 0.4860, 0.0, 0.0177};



int main(){

// Part 1 
float theta[7]={0, 0.1, -.8, 19.373155, -0.523592, 1.5707963, 3.141592};
short theta_q[7]={ 0 };


float res_fcos1[7] = { 0 }; 
float res_fcos2[7] = { 0 }; 
short res_icos[7] = { 0 }; 



int i =0;
printf("The cosine estimated values using the 3 functions are:\n\n");
printf("theta           fcos1(theta)    fcos2(theta)   icos(theta)\n\n");


for (i; i < 7; i++) {
	
	res_fcos1[i] = fcos1(theta[i]);
	res_fcos2[i] = fcos2(theta[i]);
	res_icos[i] =  icos(float_to_fixed(theta[i], INPUT_FIXED_POINT_BITS));
	
	printf("%f	%f	%f	%f\n", theta[i], res_fcos1[i], res_fcos2[i], fixed_to_float(res_icos[i], OUTPUT_FIXED_POINT_BITS));
	
}


//part 2:

float signal[SIGNAL_LEN] = { 0 };
short fixed_signal[SIGNAL_LEN] = { 0 };

short bcoeff[ FILTER_ORDER + 1] = { 0 };
short acoeff[ FILTER_ORDER + 1] = { 0 };

short filtered[SIGNAL_LEN] = { 0 };
float float_filtered[SIGNAL_LEN] = { 0 };



	

random_signal(signal);
//converting input signal array to an array of fixed point values:
i= 0;
for (i; i < SIGNAL_LEN; i++) {		
	fixed_signal[i] = float_to_fixed(signal[i], 14) ;
}


//converting coefficient arrays to arrays of fixed point values:

float b[] = {0.0941, 0.3759, 0.5639, 0.3759, 0.0940};
float a[] = { 1.0, 0.0, 0.4860, 0.0, 0.0177};

i =0;
for (i; i < FILTER_ORDER + 1; i++) {	
	bcoeff[i] = float_to_fixed(b[i], 14);
	acoeff[i] = float_to_fixed(a[i], 14);

}

IIR_fixed(bcoeff, acoeff, fixed_signal, filtered, SIGNAL_LEN, 5) ;


FILE* IIR_OUTPUT;
fopen_s(&IIR_OUTPUT, "filtered_signal.dat", "w");

// converting output to float and saving in output file
i= 0;
for (i; i < SIGNAL_LEN; i++) {	
	float_filtered[i] = float_to_fixed(filtered[i] , 14);
	printf("%f\n", float_filtered[i]);
	fprintf(IIR_OUTPUT, "%f\n", float_filtered[i]);
}

}

float fcos1(float x)
{
float out;
out=fcoef[0];
out+=fcoef[1]*x*x;
out+=fcoef[2]*x*x*x*x;
out+=fcoef[3]*x*x*x*x*x*x;
return out;
}
	

float fcos2(float x)
{
float out,x2;
x2=x*x;
out=x2*fcoef[3];
out=(out+fcoef[2])*x2;
out=(out+fcoef[1])*x2;
out+=fcoef[0];
return out;
}


short icos(short x)
{ long cosine,z;
short x2;
z = (long)x * x;
x2 = (short)(z>>15); // x2 has x(Q14)*x(Q14)
cosine = (long)iCoef[3] * x2;
cosine = cosine >> 13;
cosine = (cosine + (long)iCoef[2]) * x2;
cosine = cosine >> 13;
cosine = (cosine + (long)iCoef[1]) * x2;
cosine = cosine >> 13;
cosine = cosine + iCoef[0];
return((short)cosine);
}


void IIR_fixed(short *coeffs_B, short *coeffs_A, short *input, short *output, int signal_length, int filterLength)
{
    short *coeffa, *coeffb;
    int n,k;
	short acc,bcc;
    //filter length = 5
    for (n = filterLength - 1; n < signal_length; n++) {

        acc = 0;
        bcc = 0;

        for (k = 0; k < filterLength; k++) {
            acc += (coeffs_A[k]) * (input[n-k]); //a[0] * x[4] +  a[1] * x[3] +  a[2] * x[2] +  a[3] * x[1] + a[4] * x[0]
            bcc += (coeffs_B[k]) * (input[n-k]); //b[0] * x[4] +  b[1] * x[3] +  b[2] * x[2] +  b[3] * x[1] + b[4] * x[0]
        }

        output[n] = bcc + acc;

    }
}



void random_signal(float* arr) {
	FILE* rand_file;
	fopen_s(&rand_file, "random.dat", "w");
	srand((unsigned int)time(NULL));
	int i =0;
	for (i; i < SIGNAL_LEN; i++)
		arr[i] = 2 * ((float)rand()) / RAND_MAX - 1;
		
	i =0;
	for (i; i < SIGNAL_LEN; i++)
		fprintf(rand_file, "%lf\n", arr[i]);
	fclose(rand_file);
}



short float_to_fixed(float input, int shift_amount)
{
    return (short)(round(input * (1 << shift_amount)));
}

float fixed_to_float(short input, int shift_amount)
{
    return ((float)input / (float)(1 << shift_amount));
}



