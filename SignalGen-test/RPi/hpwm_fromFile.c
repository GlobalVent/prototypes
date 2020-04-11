/*
hwpwm_fromFile.c

To compile:
gcc -pthread -o hwpwm_fromFile hpwm_fromfile.c -lpigpio

Usage:
sudo ./hpwm_fromFile txtWaveform fs(numeric) GPIOpin(numeric, GPIO18 default) 
sudo ./hwpwm_fromFile test.txt 200 18 
*/

#include <stdio.h>
#include <stdlib.h>
#include <pigpio.h>

#define CMH2O_TO_BAR 0.000980638; 

int main(int argc, char *argv[])
{
	
	int MAX_LEN = 1501; //Note if you set to exact length in Sa add one for escape char
	char* inFilename[256];
	FILE *fp;
	u_int32_t Ts_micros;
	int g=18, fs=200, gpioPin=18;
	//strcpy(inFilename, "test.txt");
	int status;
	int fPWM = 50000; //This should typically be at least 10x your highest frequency

	//Parse inputs
	if (argc > 1) fp = fopen(argv[1], "r");
	if (argc > 2) fs = atoi(argv[2]);
	if (argc > 3) gpioPin = atoi(argv[3]);

   printf("input file %s, fs = %d, GPIO Pin %d\n", inFilename, fs, gpioPin);


	//Init gpio
   if (gpioInitialise()<0) return -1;


//Read signal from file into array
double *xBuffer, *yBuffer;
double *tmp; //In case we run over pre-allocated array
xBuffer = malloc(MAX_LEN*sizeof(double));
yBuffer = malloc(MAX_LEN*sizeof(double));

printf("Loading file...\n");
if(!fp) {
	printf("Load file failed...\n");
	exit(1);
}

printf("Reading data...\n");
int i = 0;
int readCode = 1; // Return for fscanf = number of items returned by call, don't check inline while() bc it will skip first line
while(readCode > 0)
{
	//printf("readCode = %d\n", readCode);
	readCode = fscanf(fp,"%lf", &xBuffer[i]);
	//printf("xBuffer[%d] = %3.2lf\n", i, xBuffer[i]);

	// CONVERT FROM CMH2O TO BAR -- AJO TO-DO - functionize sensor type as input --- DIFFERENT FOR DIFFERENT SENSORS!! (*CMH2O_TO_BAR/0.14 FOR PSYS, * CMH2O_TO_BAR FOR P_RES)
	yBuffer[i] = xBuffer[i] * CMH2O_TO_BAR * 1.0/0.14;
	
	printf("xBuffer[%d] = %3.2f\n yBuffer[%d] = %3.2f\n", i, xBuffer[i], i, yBuffer[i]);

	if (i >= MAX_LEN) // If out of bounds re-allocate more memory
	{
	printf("Reallocating memory for xBuffer - AJO TODO FIX THIS");
    tmp = realloc(xBuffer, i*sizeof(double));
    xBuffer = tmp;
	tmp = realloc(yBuffer, i*sizeof(double));
	yBuffer = tmp;
    MAX_LEN = i+1;
	}
  i++;
}
fclose(fp);
printf("Done reading file...\n");


//Calculate sample period in us for main loop timing
Ts_micros = (double)(1.0/fs)*1000000; // Can subtract a little here to acct for overhead?? Ex - Ts _micros *= 0.9

printf("Outputting signal via GPIO\n");

printf("MAX_LEN = %d\n", MAX_LEN);
printf("Ts_micros = %d\n", Ts_micros); 

uint32_t actualWait[MAX_LEN];
uint32_t waitSum = 0;
//Main loop -- to-do change MAX_LEN to actual number of samples read above (i before re-init to 0) - in case nSamples < MAX_LEN ?
for(i=0;i <= MAX_LEN; i++) {
	status = gpioHardwarePWM(gpioPin, fPWM, yBuffer[i]*1e6); //1e6 is 100% duty cycle
	// Can check status != 0 here but will add more overhead, leaving out for now
	actualWait[i] = gpioDelay(Ts_micros); //Microsecond-precision
	
	//Uncomment lines below for debugging only
	//waitSum += actualWait[i];
	//printf("Actual wait on sample[%d] = %d\n", i, actualWait[i]); //Debug code only
}

	//Debug stuff - print total drift, write buffer of drifts to file
	/*
	uint32_t sumDrift = waitSum - Ts_micros*i;
	printf("Total accumulated drift in S = %d\n", sumDrift);
	FILE * dbfp = fopen("debugOut.txt", "wb");
	if(dbfp == NULL) {
	 printf("Error opening file\n"); 	
	 } else {
		printf("Writing debug info to file\n");
		for(i=0; i <= MAX_LEN; i++) {		
		fprintf(dbfp, "%d\n",actualWait[i]);
		}
		fclose(dbfp);
	}
	*/

	//Close out GPIO
	gpioHardwarePWM(gpioPin, 0, 0);
	gpioTerminate();
   
	printf("Press return to exit\n");
	getchar();
   

}
