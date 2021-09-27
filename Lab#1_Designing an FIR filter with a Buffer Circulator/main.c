#include "fdacoefs.h"
#include <stdio.h>

#define INPUT_SIZE 24000
 // input buffer:
float x[BL];

float y;
int i, xi, latest_ind=0;

float FIR_filter2(float newsample)
{
	++latest_ind;
	if (latest_ind == BL)
	{
		latest_ind=0;
	}
	
	x[latest_ind] = newsample;
	y = 0;
	xi = latest_ind;
	
	for(i = 0; i <  BL; i++)
	{ 
		y = y + B[i] * x[xi];
		--xi;
		if (xi == -1){
		 xi = BL - 1;
		}		
	}
	return y;
}



int main() {
	
	
	// reading first sine wave file 
    FILE *file = fopen("sinus_1.txt", "r");
    float sin1[INPUT_SIZE];

    int i=0;
    float num;
    while(fscanf(file, "%f", &num) > 0) {
        sin1[i] = num;
        i++;
	}
	fclose(file);


	// reading second sine wave file
    file = fopen("sinus_2.txt", "r");
    float sin2[INPUT_SIZE];

	i = 0;
    while(fscanf(file, "%f", &num) > 0) {
        sin2[i] = num;
        i++;
	}
	fclose(file);
	


	
	// filtering first sine wave
	float out_sin1[INPUT_SIZE], out_sin2[INPUT_SIZE];
	
	for (i = 0; i < INPUT_SIZE; i++)
		out_sin1[i] = FIR_filter2(sin1[i]);	

	// filtering second sine wave
	for (i = 0; i < INPUT_SIZE; i++)
		out_sin2[i] = FIR_filter2(sin2[i]);

		
	// storing output wave on a .txt file
	
	file = fopen("sin1_output.txt", "w+");
	i = 0;
	while (i < INPUT_SIZE)
	{
		fprintf(file, "%f ", out_sin1[i]);
		i++;
	}
	fclose(file);

	
	
	file = fopen("sin2_output.txt", "w+");
	i = 0;
	while (i < INPUT_SIZE)
	{
		fprintf(file, "%f ", out_sin2[i]);
		i++;
	}
	fclose(file);
	
	
	
	return 0;
}
