
This [link](https://www.eecs.tufts.edu/~dsculley/tutorial/rc/rc3.html#:~:text=The%20RC%20time%20constant%20is,the%20resistance%20R%2C%20in%20Ohms) is a great resource for understanding how one can measure a capacitor using the RC time constant.


### Chat GPT TALK

# ChatGPT gave lots of good insight on the topic of measurong electorcial comopnents. Below is some out my response when I got from the chatbox when asking it question regaruding how to measure capacitors and resistors. 

For the capacitor range you specified (0.001 μF to 10 μF), a good resistor value to use would be around 1 kΩ to 100 kΩ. 
This will give you a time constant between 10 μs and 10 ms, which is suitable for this range of capacitance values.
If you use a resistor value of 10 kΩ, for example, the time constant would be: τ = RC = 10 kΩ x 0.001 μF = 10 μs
This would allow you to measure capacitors as small as 0.001 μF with a resolution of 10 μs.
On the other hand, if you use a resistor value of 100 kΩ, the time constant would be: τ = RC = 100 kΩ x 10 μF = 1 s
This would allow you to measure capacitors as large as 10 μF with a resolution of 1 second. Keep in mind that the accuracy 
of your capacitance meter will depend on the accuracy of the components used and the circuit design, so you may need to calibrate 
your meter for more precise measurements.

Using multiple circuits with different resistor values can be a good approach to increase the measurement range and resolution of 
your capacitance meter. By using different resistor values, you can create multiple RC time constants and cover a wider range of 
capacitance values. For example, you could use three circuits with resistors of 1 kΩ, 10 kΩ, and 100 kΩ, respectively. This would 
give you time constants of 1 μs, 10 μs, and 100 μs, covering a capacitance range from 0.001 μF to 10 μF with a resolution of 1 μs.
To implement multiple circuits, you can use a multiplexer or switch to select the appropriate resistor for each measurement. You can 
then measure the charging time for each circuit and calculate the capacitance value using the appropriate time constant.
Keep in mind that using multiple circuits can increase the complexity and cost of your capacitance meter, and you may need to consider
factors such as switching speed and signal interference. However, it can be a useful approach to improve the accuracy and versatility of 
your measurements.



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

int R1(int i) {
	int loadResistor;
	switch (i) {
		case 1:
			FIO0DIR |= (1 << 9);
			FIO0DIR &= ~(1 << 8) & ~(1 << 7) & ~(1 << 6) & ~(1 << 0);
	    	printf("case 1 \n");
	    	loadResistor = 670;
			break;
		case 2:
			FIO0DIR |= (1 << 8);
			FIO0DIR &= ~(1 << 9) & ~(1 << 7) & ~(1 << 6) & ~(1 << 0);
	    	printf("case 2 \n");
	    	loadResistor = 9860;
			break;
		case 3:
			FIO0DIR |= (1 << 7);
			FIO0DIR &= ~(1 << 8) & ~(1 << 9) & ~(1 << 6) & ~(1 << 0);
	    	printf("case 3 \n");
	    	loadResistor = 9927;
			break;
		case 4:
			FIO0DIR |= (1 << 6);
			FIO0DIR &= ~(1 << 8) & ~(1 << 7) & ~(1 << 9) & ~(1 << 0);
	    	printf("case 4 \n");
	    	loadResistor = 98520;
			break;
		case 5:
			FIO0DIR |= (1 << 0);
			FIO0DIR &= ~(1 << 8) & ~(1 << 7) & ~(1 << 6) & ~(1 << 9);
	    	printf("case 5 \n");
	    	loadResistor = 470000;

			break;
	}
	return 	loadResistor;
}


int main () {

    float digitalV;
    int i = 0;
	float resistorValue = 0;
	int loadResistor = 0;

	config();

	R1(2);

    while (1) {

    	digitalV = triggerAtoD();


		if (digitalV <= 375 || digitalV >= 3300) { //digitalV <= 1433 || digitalV >= 2662
			i++;
			if (i > 5) {
				i = 1;
			}
			loadResistor = R1(i);
		}
//		else if (digitalV > 2048) {
//			loadResistor = R1(3);
//
//		} else {
//			loadResistor = R1(2);
//
//		}



		resistorValue = (-1 * digitalV * loadResistor) / (digitalV - 4096);
    	printf("i == %d     %f ohms \n", i, resistorValue);
    	printf("%f digitalV \n", digitalV);



	}
}

