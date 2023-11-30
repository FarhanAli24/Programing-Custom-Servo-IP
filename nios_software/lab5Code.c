#include "io.h"
#include <stdio.h>
#include "system.h"
#include "alt_types.h"
#include "sys/alt_irq.h"


// create standard embedded type definitions
typedef   signed char   sint8;              // signed 8 bit values
typedef unsigned char   uint8;              // unsigned 8 bit values
typedef   signed short  sint16;             // signed 16 bit values
typedef unsigned short  uint16;             // unsigned 16 bit values
typedef   signed long   sint32;             // signed 32 bit values
typedef unsigned long   uint32;             // unsigned 32 bit values
typedef         float   real32;             // 32 bit real values


//set up pointers to peripherals
//unsigned char* LedPtr      = (unsigned char*)LEDS_BASE;

volatile uint32* swPtr	   = (uint32*)SWITCHES_BASE;
volatile uint8* hex0Ptr	   = (uint8*)HEX0_BASE;
volatile uint8* hex1Ptr	   = (uint8*)HEX1_BASE;
volatile uint8* hex2Ptr	   = (uint8*)HEX2_BASE;
volatile uint8* hex4Ptr	   = (uint8*)HEX4_BASE;
volatile uint8* hex5Ptr	   = (uint8*)HEX5_BASE;

volatile uint32* servo 	   = (uint32*)MY_CUSTOM_IP_0_BASE;//using 32 bits for servo cordinates position

volatile uint32* keyPtr	   = (uint32*)PUSHBUTTONS_BASE;


volatile int start = 0;
volatile int stop = 0;
volatile unsigned int tens = 0;
volatile unsigned int ones = 0;

volatile unsigned int hundreds = 0;
volatile unsigned int tens2 = 0;
volatile unsigned int ones2 = 0;
volatile int key = 0;
volatile uint8 switches = 0;
					                 //0     1    2     3     4	    5     6     7     8     9
volatile unsigned int numbers[10] = {0x040,0x079,0x024,0x030,0x019,0x012,0x002,0x078,0x000,0x010}; //Containts all numbers
/*****************************************************************************/
/* Interrupt Service Routine                                                 */
/*   Determines what caused the interrupt and calls the appropriate          */
/*  subroutine.                                                              */
/*                                                                           */
/*****************************************************************************/

 void key_isr(void *context)//key interrupts
 {


	key = *(keyPtr+3);//offset 3 to enable edgecapture

    switches = *swPtr; //using Switches

	if(key & 0x08)//if key 3 is pressed
	{
		//if switch value is 45 or greater and if switch value is 90 or less
		if((switches >= 0x2D) & (switches <= 0x5A))
		{
			start = switches;

			tens = switches/10;
			*hex5Ptr = numbers[tens];

			ones = switches % 10;
			*hex4Ptr = numbers[ones];
		}

		else
		{
			start = 0x0;
			*hex4Ptr = numbers[0];
			*hex5Ptr = numbers[0];
		}
	}

	else if(key & 0x04)//if key 2 is pressed
	{
		//if switch value is 90 or greater and if switch value is 135 or less
		if((switches >= 0x5A) & (switches <= 0x87))
		{
			stop = switches;

			hundreds = switches/100;
			*hex2Ptr = numbers[hundreds];

			tens2 = (switches/10) % 10;
			*hex1Ptr = numbers[tens2];

			ones2 = switches % 10 % 10;
			*hex0Ptr = numbers[ones2];

		}

		else
		{
			stop = 0;
			*hex0Ptr = numbers[0];
			*hex1Ptr = numbers[0];
			*hex2Ptr = numbers[0];
		}
	}
	*(keyPtr + 3) = 0x00;
    return;
}

void servo_isr(void *contect)
{
	*(servo + 1) = (((25000/45)*start)+25000);
	*(servo + 0)  = (((25000/45)*stop )+25000);
}

int main(void)
/*****************************************************************************/
/* Main Program                                                              */
/*   Enables interrupts then loops infinitely                                */
/*****************************************************************************/
{
	*(keyPtr + 2) = 0x00c; //using Key2 and key 3 as interrupt enable. Offset is 2 for enabling interruptmask for IRQ
//	*(keyPtr + 2) = 0x008; //using Key3 as interrupt enable. Offset is 2 for enabling interruptmask for IRQ

	*(servo + 0) = 50000;//init low value
	*(servo + 1) = 100000;//init high value



	alt_ic_isr_register(PUSHBUTTONS_IRQ_INTERRUPT_CONTROLLER_ID,PUSHBUTTONS_IRQ,key_isr,0,0);
	alt_ic_isr_register(MY_CUSTOM_IP_0_IRQ_INTERRUPT_CONTROLLER_ID,MY_CUSTOM_IP_0_IRQ,servo_isr,0,0);

    while(1)
    {
    	//*(servo + 1) = 50000;//init low value
    	//*(servo + 0) = 100000;//init high value
    }
    return 0;
}
