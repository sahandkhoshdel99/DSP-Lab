#include <math.h>
#include "fft.c"
#include "Complex.c"


int main()
{
	int i;
	
	#setting sampling frequency
	
	float fs = 16000.0;
	int N = 1024


	complex x[N];
	
	for (i = 0; i < N; i++){
		x[i].x = sin(2*i*PI/8.0);
		x[i].y = 0;
	}
	
	
	fft(x, 	N, fs);
	for(i = 0; i < N; i++)
	{
		printf("%f ", ccabs(x[i]));
	}
	
	
	# opening empty output file 
	FILE
	* output_file = fopen("dft_output.txt", "w+");
	
	
	# writing fft result in output file
	i = 0;
	while (i < N)
	{
		fprintf(output_file, "%f ", ccabs(x[i]));
		i++;
	}
	
	fclose(file);
	return 0;
}
