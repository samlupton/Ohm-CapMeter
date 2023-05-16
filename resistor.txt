/*
===============================================================================
 Name        : ohmmeter.c
 Author      : Nhi and Sam
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/
#include <stdio.h>

//Define Port 0
#define FIO0DIR (*(volatile unsigned int *)0x2009c000)
#define FIO0PIN (*(volatile unsigned int *)0x2009c014)
#define PINMODE0 (*(volatile unsigned int *)0x4002c040)
#define PINMODE1 (*(volatile unsigned int *)0x4002c044)

// Registers
#define PINSEL0 (*(volatile unsigned int *)0x4002C000)
#define PINSEL1 (*(volatile unsigned int *)0x4002C004)
#define PINSEL3 (*(volatile unsigned int *)0x4002C00C)
#define PCONP (*(volatile unsigned int*)0x400FC0C4)
#define AD0CR (*(volatile unsigned int*)0x40034000)
#define AD0GDR (*(volatile unsigned int*)0x40034004)

void config() { // pin configuration

    // configure PINSEL to select AD0.0
    PINSEL1 |= (1<<14);
    PINSEL1 &= ~(1<<15);
    PCONP |= (1<<12);
    AD0CR |= (1<<21);
    // disable pull-up/pull-down
    PINMODE0 &= ~(1<<0) & ~(1 << 10) & ~(1<<14) & ~(1<<16) & ~(1<<18);
    PINMODE0 |=	(1<<1) | (1 << 11) | (1<<15) | (1<<17) | (1<<19);
    PINMODE1 &= ~(1<<14);
    PINMODE1 |= (1<<15);
    // set to HIGH
    FIO0PIN |= (1<<0) | (1<<6) | (1<<7) | (1<<8) | (1<<9);
}

int triggerAtoD() { // analog to digital conversion

	int result = 0;

	AD0CR |= (1<<0); // set AD0CR to be used as the input
	AD0CR &= ~(1<<16); // set BURST bit to 0
	AD0CR |= (1<<24); // set bit 24 to 1 to start conversion
	while (((AD0GDR >> 31) & 1) != 1) {} // wait for DONE bit to become 1
	AD0CR &= ~(1<<24); // clear bit 24

    // read result (bits 4-15 in AD0GDR)
	result = (AD0GDR >> 4) & 0xfff;

    return result;
}

void wait(float seconds) {
    volatile int count = seconds * 21.33e6 / 24;
    while (count > 0) count--;
}

double R1(int i) {
	double loadResistor;
	switch (i) {
		case 1:
			FIO0DIR |= (1 << 9);
			FIO0DIR &= ~(1 << 8) & ~(1 << 7) & ~(1 << 6) & ~(1 << 0);
	    		loadResistor = 2970.0;
			break;
		case 2:
			FIO0DIR |= (1 << 8);
			FIO0DIR &= ~(1 << 9) & ~(1 << 7) & ~(1 << 6) & ~(1 << 0);
	    		loadResistor = 1778.0;
			break;
		case 3:
			FIO0DIR |= (1 << 7);
			FIO0DIR &= ~(1 << 8) & ~(1 << 9) & ~(1 << 6) & ~(1 << 0);
	    		loadResistor = 9927.0;
			break;
		case 4:
			FIO0DIR |= (1 << 6);
			FIO0DIR &= ~(1 << 8) & ~(1 << 7) & ~(1 << 9) & ~(1 << 0);
	    		loadResistor = 98520.0;
			break;
		case 5:
			FIO0DIR |= (1 << 0);
			FIO0DIR &= ~(1 << 8) & ~(1 << 7) & ~(1 << 6) & ~(1 << 9);
	    		loadResistor = 388200.0;
			break;
	}
	return 	loadResistor;
}

int main () {

    	float digitalV;
    	int i = 0;
	float resistorValue = 0;
	double loadResistor = 0;
	config();
	R1(3);
	
   	while (1) {

    		digitalV = triggerAtoD();
		
		if (digitalV <= 100 || digitalV >= 3500) {  
			i++;
			if (i > 5) {
				i = 1;
			}
			loadResistor = R1(i);
		}

		resistorValue = (-1 * digitalV * loadResistor) / (digitalV - 4096.0);
    		printf("%f ohms \n", resistorValue);
	}
}
