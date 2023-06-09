#include <stdio.h>

//Define Port 0
#define FIO0DIR (*(volatile unsigned int *)0x2009c000)
#define FIO0PIN (*(volatile unsigned int *)0x2009c014)
#define PINMODE0 (*(volatile unsigned int *)0x4002c040)

// Registers
#define PINSEL0 (*(volatile unsigned int *)0x4002C000)
#define PINSEL1 (*(volatile unsigned int *)0x4002C004)
#define PINSEL3 (*(volatile unsigned int *)0x4002C00C)
#define PCONP (*(volatile unsigned int*)0x400FC0C4)
#define AD0CR (*(volatile unsigned int*)0x40034000)
#define AD0GDR (*(volatile unsigned int*)0x40034004)
#define T0TCR (*(volatile unsigned int*)0x40004004)
#define T0TC (*(volatile unsigned int*)0x40004008)
#define PCLKSEL0 (*(volatile unsigned int *)0x400FC1A8)
#define PINMODE1 (*(volatile unsigned int *)0x4002C044)

// pin configuration
void config() { 

    	// configure PINSEL to select AD0.0
    	PINSEL1 |= (1<<14);
	PINSEL1 &= ~(1 << 15);
	PCONP |= (1<<12);
	AD0CR |= (1<<21);
	FIO0DIR |= (1 << 2);
	PCLKSEL0 |= (1 << 24);
	PCLKSEL0 &= ~(1 << 25); // set PCLK_ADC = CCLK
	AD0CR &= ~(1<<8); // set CLKDIV = 1;

    	// disable pull-up/pull-down
	PINMODE0 &= ~(1 << 4);
	PINMODE0 |= (1 << 5);
	PINMODE1 &= ~(1 << 14);
	PINMODE1 |= (1 << 15);
}

// analog to digital conversion
int triggerAtoD() { 

	int result;
	
	AD0CR |= (1 << 0); // set AD0CR to be used as the input
	AD0CR &= ~(1 << 16); // set BURST bit to 0
	AD0CR |= (1 << 24); // set bit 24 to 1 to start conversion
	
	while (((AD0GDR >> 31) & 1) != 1) {
	} // wait for DONE bit to become 1
	
	AD0CR &= ~(1 << 24); // clear bit 24

	// read result (bits 4-15 in AD0GDR)
	result = (AD0GDR >> 4) & 0xfff;
	
	return result;
}

void wait(float seconds) {
	volatile int count = seconds * 21.33e6 / 24;
	while (count > 0) count--;
}

double getTime() {
	double time = 0;
	FIO0PIN &= ~(1<<2);
	wait(4.5);
	T0TCR |= (1 << 0); // start timer
	int start = T0TC; // note starting time
	FIO0PIN |= (1<<2);

	while (triggerAtoD() < 2530) {}
	// checking if voltage is 2/3 of VCC (3.3V)

	int stop = T0TC; // note stop time
	T0TCR &= ~(1<<0); // stop timer


	time = stop - start;
	printf("Total Time: %f \n", time);

	return time;
}

int main() {
	double capacitorValue = 0;
	config();

	while (1) {
		// .034097 is laten capacitance 1965
		capacitorValue = (getTime() / 1965.0)  - .034097; 
		
		printf("%f uf \n", capacitorValue);
		FIO0PIN &= ~(1 << 2);
		wait(1.0);
	}
}

