#include <stdio.h>
#include <math.h>
#include "fft.c"
#include "Complex.c"


# define N 1024
# define L 8
# define len 80000

// function declarations:
void ISR(float* a, float* b, complex* ping, complex* pong);
void coef_generator(float* sig, float* a, float* b, int fr_n);
void cosine(float* s);

char cos_output[] = "cosine_fft.dat";
char sin_output[] = "sine_fft.dat";
char square_output[] = "square_fft.dat";
char noise_output[] =  "noise_fft.dat";

int main() {
	

	// variable declarations:	
	double spectrum[N/2+1] = {0};
	double buffer[N / 2 + 1] = { 0 };
	float a[N] = { 0 };
	float b[N] = { 0 };
	
	complex ping[N];
	complex pong[N];
	
	char sine_test[] = "sine.txt";
	char square_test[] = "square.txt";
	char noise_test[] = "noise.txt";
	
	float signal[len] = {0};
	//cos(signal);
	
	FILE* file;
	fopen_s(&file, cos_output, "w");

	//code starts here:
	
	int j = 0;
	for (j; j < floor(len / (2 * N)); j++) {
		coef_generator(signal, a, b,j);
		ISR(a, b, ping, pong);
		fft(pong, 1024, 1);
		
		int i = 0;
		for (i; i < N / 2 + 1; i++) {
			buffer[i] += (ccabs(pong[i])*ccabs(pong[i]) + ccabs(pong[N -1-i])*ccabs(pong[N-1-i])) / 2;
			
			if ((j - 1) % L == 0){
				spectrum[i] += buffer[i] / (L* N);
				fprintf(file, "%lf\n", spectrum[i]);

				buffer[i] = 0;
			}
		}
	
	fclose(file);
	return 0;
}
	
void coef_generator(float* signal, float* a, float* b, int fr_n) {
	
	// fr_n is the frame number (it divides the signal into frames and return coressponding samples:
	
	// gets the first 2N samples of the test signal and divides into half 
	int i = 0;
	for (i; i < 2 * N; i++) {
		// the first half samples generate a coefficients:
		if (i < N)
			a[i] = signal[fr_n * N + i];
		
		// the second half samples generate b coefficients:
		else
			b[i - N] = signal[fr_n * N + i];
	}
}



void ISR (complex* ping, complex* pong, float* a, float* b) {
	int i = 0;
	for (i; i<N; i++) {
		
		// the real part of each of ping and pong signals is going to be the real signal a 
		ping[i].x = a[i];
		
		// the imaginary part of each of ping and pong signals is going to be the real signal b 
		ping[i].y = b[i];
		 
		pong[i].x = a[i];
		pong[i].y = b[i];
	}
}

void cosine(float* s) {
	int i= 0;
	for (i; i < len; i++){
		s[i] = 10000 * cos(i * 100 * 2 * PI / 1024);
	}
}
}
