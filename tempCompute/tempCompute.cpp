// tempCompute.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

double g_dTempCompute[21] = { 0 };

char* szString[21] = {
	"200  160",
	"200  200",
	"250  200",
	"250  250",
	"320  250",
	"320  320",
	"400  250",
	"400  320",
	"500  320",
	"500  400",
	"630  320",
	"630  400",
	"800  320",
	"800  400",
	"1800  900",
	"1250  500",
	"1250  400",
	"1000  400",
	"1600  800",
	"1800  400",
	"1600  400"
};

VOID compute_J_2f_1()
{
	int nTempNumber = 0;
	g_dTempCompute[nTempNumber++] = (0.2+0.16)*2*(1.3+0.4+1.9);										// 200 X 160
	g_dTempCompute[nTempNumber++] = (0.2+0.2)*2*(0.5+1.032+2.501+1.665+2.301+0.841+2.141+1.352);		// 200 X 200
	g_dTempCompute[nTempNumber++] = (0.25+0.2)*2*(0.665+0.665+0.5);									// 250 X 200
	g_dTempCompute[nTempNumber++] = (0.25+0.25)*2*(3.701+2.75+0.975+\
		2.025+2.025+1.95+0.725+0.575+1.725+1.725+2.165+0.5+2.425+1.704+\
		0.5+0.325+2.875+0.5+0.24+1.575+0.690+10.904+1.59+1.59+4.94+0.5+1.618+0.06+0.5+1.008);		// 250 X 250
	g_dTempCompute[nTempNumber++] = (0.32+0.25)*2*(2.978+3.669+0.64+4.344+1.345+1.461+4.008+\
		1.090+2.26+2.26+0.81+1.571+2.079+0.64+1.044+0.64+0.469+\
		0.804+5.88+0.64+2.338+0.419+0.419+0.64+5.06);													// 320 X 250
	g_dTempCompute[nTempNumber++] = (0.32+0.32)*2*(2.11+2.605+4.661+2.115+4.604+0.589+0.64+\
		5.169+5.175+3.898+1.65+3.23+2.58+2.438+7.34);												// 320 X 320
	g_dTempCompute[nTempNumber++] = (0.4+0.25)*2*(2.29+0.783);											// 400 X 250
	g_dTempCompute[nTempNumber++] = (0.4+0.32)*2*(4.789+7.964+2.325+1.34+0.8+0.21+0.8+1.331+4.971+\
		0.8+0.8+4.918+1.585+0.765+2.885+1.261+0.8+0.8+1.984+0.614+1.745+0.8+2.69+4.35);				// 400 X 320
	g_dTempCompute[nTempNumber++] = (0.5+0.32)*2*(2.908+2.042+3.509+1+\
		1+0.3+1+0.43+0.67+2.165);																		// 500 X 320
	g_dTempCompute[nTempNumber++] = (0.5+0.4)*2*(1+2.286+1+0.5+1+4.874+2.44);						// 500 X 400
	g_dTempCompute[nTempNumber++] = (0.6+0.32)*2*(2.765+1.26+2.027+2.429+2.514+\
		2.364+2.269+4.677+1.561);																		// 630 X 320
	g_dTempCompute[nTempNumber++] = (0.6+0.4)*2*(3.132+1.26+3.196+2.671+1.26+5.121);				// 630 X 400
	g_dTempCompute[nTempNumber++] = (0.8+0.32)*2*(4.05+3.345+1.6+9.414+3.996);							// 800 X 320
	g_dTempCompute[nTempNumber++] = (0.8+0.4)*2*(4.403+0.192+1.6+1.6+0.426+3.662+\
		1.6+1.6+2.852+1.6+6.62);																	// 800 X 400
	g_dTempCompute[nTempNumber++] = (1.8+0.9)*2*(1.175);												// 1800 X 900
	g_dTempCompute[nTempNumber++] = (1.25+0.5)*2*(7.61+2.5+5.6);									// 1250 X 500
	g_dTempCompute[nTempNumber++] = (1.25+0.4)*2*(2.725+1.52);											// 1250 X 400
	g_dTempCompute[nTempNumber++] = (1.0+0.8)*2*(1.296);											// 1000 X 400
	g_dTempCompute[nTempNumber++] = (1.6+0.8)*2*(5.972);												// 1600 X 800
	g_dTempCompute[nTempNumber++] = (1.8+0.4)*2*(0.561);											// 1800 X 400
	g_dTempCompute[nTempNumber] = (1.6+0.4)*2*(8.109+1.775);											// 1600 X 400

	return;
}

int main(int argc, char* argv[])
{
	compute_J_2f_1();
	double nAllOfNumber = NULL;
	for (int i = 0; i < 21; i++)
	{
		printf("%s -->  %.3lf㎡ \n", szString[i], g_dTempCompute[i]);
		nAllOfNumber += g_dTempCompute[i];
	}
	printf("风管总计：%.3lf㎡\n", nAllOfNumber);
	system("pause");
	return 0;
}