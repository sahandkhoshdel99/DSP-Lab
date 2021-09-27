#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <complex>
#include <cmath>
#define pi  3.14
using namespace std;


// filter coefficients:

double Reg_1[3], Reg-2[3], Reg_3[3];  

const float A[3][3] = {{	1,   -1.07,   0.84}, {1, 0.54, 0.82},{1, -0.17, 0.013}}; 
const float B[3][3] = { {1, -0.69, 1}, {1, 0.018, 1},{1,  -0.35, 1} }; 

const float G[3] = {0.603, 0.603, 0.96};

// Direct form II implementation:
double DF_2(int k, double x)
{
	double y;
	Reg0[k] = x - A[k][1] * Reg1[k] - A[k][2] * Reg2[k];
	y = B[k][0]*Reg_1[k] + A[k][2]*Reg_3[k] +  B[k][1]*Reg_2[k] ;
	Reg_3[k] = Reg_2[k];
	Reg_2[k] = Reg_1[k];

 	return(y);
}



vector<float> IIR(vector<float> Input, int NumSigPts)
{
	double y=0;
	vector<float> Output;
	for(int j=0; j<3; j++)
  	{
   		Reg0[j] = 0;
   		Reg1[j] = 0;
   		Reg2[j] = 0;
  	}

 	for(int j=0; j < NumSigPts; j++)
  	{	
		y = DF_2(0, Input[j]*G[0]);
		y = DF_2(1, y*G[1]);
		y = DF_2(2, y*G[2]);
   		Output.push_back(y);
  	}
	return Output;
}




// code for calculating fft: 
vector<float> fft(vector<float> x)
{
	int N = x.size();
	complex<float> W (cos(2*pi/N), -sin(2*pi/N));
	complex<float> temp, W_n, coef;
	vector<float> X;
	
	for(int k=0; k < N; k++)
	{
		temp = (0, 0);
		for(int n=0; n < N; n++)
		{
			W_n =  pow(W, k*n);
			coef = (real(x[n])*real(W_n)-imag(x[n])*imag(W_n), real(x[n])*imag(W_n)+imag(x[n])*real(W_n));
			temp = real(temp)+real(coef), imag(temp)+imag(coef);
			X.push_back(abs(temp));
		}
	}
	return X;
}



// function for constructing additive white gaussian noise:
// needs a random engine generator,normal distribution,sampling from the distribution,and gathering samples together to generate the noise:

vector<float> awgn_generator(int t, int Fs, float mu, float sigma)
{
	// random engine for pushing noraml samples:
	unsigned seed = chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
	
	// constructing a normal distribution :
	normal_distribution<float> distribution (mu, sigma);

	// t*Fs is the number of total samples:	
	vector<float> awgn;
	// Loop for sampling from the normal distrbution and constructing awgn:
	for(int i=0; i < t*Fs; ++i){
		awgn.push_back(distribution(generator));
	}	
	return awgn;	 
}



int main()
{
	// SECTION II : NOISE (MU = 0 / SIGMA  = 1)
	int i;
	int t = 1; 
	int Fs = 100;
	int mu = 0;
	int sigma = 1;
	
	//constructing noise, applying IIR bandstop filter and getting the fourier transform ( spectrum ) of the filtered noise.
	vector<float> noise =  awgn_generator(Fs, t, mu, sigma);
	vector<float> filtered_noise = IIR(noise, noise.size());
	vector<float> fft_filtered_noise = fft(filtered_noise);
	
	//
 	FILE *f ;
	f = fopen("noise_100.txt","w");
  	for(int i=0;i<noise.size();i++)
	{
		fprintf(f,"%f\n",noise[i]);
	}
	fclose(f);
		f = fopen("filtered_noise_100.txt","w");
  	for(int i=0;i<fft_filtered_noise.size();i++)
	{
		fprintf(f,"%f\n",fft_filtered_noise[i]);
	}
	fclose(f);
	
	
	
	// SECTION II : NOISE (MU = 10 / SIGMA  = 2)
	
	int mu = 10;
	int sigma = 2;
	
	//constructing noise, applying IIR bandstop filter and getting the fourier transform ( spectrum ) of the filtered noise.
	vector<float> noise =  awgn_generator(Fs, t, mu, sigma);
	vector<float> filtered_noise = IIR(noise, noise.size());
	vector<float> fft_filtered_noise = fft(filtered_noise);
	
	//
 	FILE *f ;
	f = fopen("noise_100_2.txt","w");
  	for(int i=0;i<noise.size();i++)
	{
		fprintf(f,"%f\n",noise[i]);
	}
	fclose(f);
		f = fopen("filtered_noise_100_2.txt","w");
  	for(int i=0;i<fft_filtered_noise.size();i++)
	{
		fprintf(f,"%f\n",fft_filtered_noise[i]);
	}
	fclose(f);
	
	
return 0;
}
