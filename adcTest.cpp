// Stan Okrasinski     							10/25/11
// TEST program, sets up ADC to be triggered from
// TIMER1 CC1 event. Board LED will flash after
// 32000 ADC conversions, and the timer is set
// for a frequency of 32 kHz. The LED should change
// state at a rate of 1 Hz. Note the rate of ADC 
// conversion is determined, in this case, more by
// the trigger rate than the ADC sample time.
// Functions in loop serve as a test mechanism, so
// timer-based sampling does not depend on 
// interrupts or recurrent cycle-using code.

#include "wirish.h"
#include "adc.h"
#include "timer.h"

int count;													// Count of ADC conversions											
uint32 adc_sequence=0;									// ADC sequence to be pushed to bit registers
int toggle = 1;											// Keep track of LED toggle status

uint32 calc_adc_sequence(uint8 adc_sequence_array[6]);

void setup() {
	count=0;

	int period = 1124;									// Period of ADC conversion
	int duty = 562;										// Moment of conversion in period

	pinMode(BOARD_LED_PIN, OUTPUT);					// Configure LED as output

	(TIMER1->regs).adv->CR2 |= 1<<8;					// Set OIS1. Necessary for timer to trigger ADC conversion
	(TIMER1->regs).adv->CR2 = bitClear((TIMER1->regs).adv->CR2,9);    // Clear OIS1N. Also necessary
	timer_set_mode(TIMER1, 1, TIMER_PWM);			// Set up Timer 1, Channel 1 to operate in PWM mode
	timer_pause(TIMER1);									// Temporarily stop the timer while configuring
	timer_set_count(TIMER1, 0);						// Reset the timer count to 0
	timer_set_reload(TIMER1, period);				// Reload, Compare, and Prescaler values set to 
	timer_set_compare(TIMER1, 1, duty);				// produce a frequency of 32 kHz
	timer_set_prescaler(TIMER1, 1);					// Set no prescaler
	//TIMER1->regs->CR2 

	adc_calibrate(ADC1);									// Calibrate ADC. Should occur once at code execution
	adc_set_sample_rate(ADC1, ADC_SMPR_71_5);		// ADC sample time of 71.5 ADC cycles
	adc_set_exttrig(ADC1, 1);							// ADC expects an external trigger....
	adc_set_extsel(ADC1, ADC_ADC12_TIM1_CC1);		// ... from TIMER1 CC1
	adc_set_reg_seqlen(ADC1, 1);						// ADC makes a single conversion

	uint8 ADC1_Sequence[]={10,0,0,0,0,0};			// ADC converts voltage on ADC pin 10 (D7)
	ADC1->regs->SQR3 |= calc_adc_sequence(ADC1_Sequence);	// Converts ADC sequence to bit register notation

	adc_enable(ADC1);										// Enables ADC
	timer_resume(TIMER1);								// Enables timer
}

void loop() {
	if((ADC1->regs->SR & 1<<1) == 2)					// If ADC is finished with a conversion
	{		
		ADC1->regs->SR ^= 1<<1;							// Reset EOC bit
		count++;												// Increment counter
		if(count==32000)									// If number of conversions reaches 32000
		{
			count = 0;										// Reset count
			digitalWrite(BOARD_LED_PIN, toggle);	// Toggle LED
			toggle ^= 1;									// Track LED state
		}
	}
}

// Force init to be called *first*, i.e. before static object allocation.
// Otherwise, statically allocated objects that need libmaple may fail.
__attribute__((constructor)) void premain() {
    init();
}

int main(void) {
    setup();

    while (true) {
        loop();
    }

    return 0;
}

// Function borrowed from Leaf Labs example DualRegSample.pde, coded by
// Samtal 
// url: https://github.com/mbolivar/DualRegSample/blob/master/DualRegSample.pde

uint32  calc_adc_sequence(uint8 adc_sequence_array[6])
{
  adc_sequence=0;

  for (int i=0;i<6;i++)     // There are 6 available sequences in each SQR3 SQR2, and 4 in SQR1.
  {
    /*This function converts the array into one number by multiplying each 5-bits channel numbers 
     by multiplications of 2^5
     */
    adc_sequence=adc_sequence + adc_sequence_array[i]*pow(2,(i*5));  
  } 
  return adc_sequence;
}   //end calc_adc_sequence
