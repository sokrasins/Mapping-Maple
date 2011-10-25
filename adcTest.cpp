// Stan Okrasinski     10/24/11
// TEST program, sets up ADC

#include "wirish.h"
#include "adc.h"
#include "timer.h"

int count;
uint32 adc_sequence=0;
int toggle = 1;

uint32 calc_adc_sequence(uint8 adc_sequence_array[6]);

void setup() {
	count=0;

	int period = 1124;
	int duty = 562;

	pinMode(BOARD_LED_PIN, OUTPUT);

	TIMER1->regs->CR2 |= 1<<8;			// TODO: set OIS1, reset OIS1N
	timer_set_mode(TIMER1, 1, TIMER_PWM);	// Set up Timer 1, Channel 1 to operate in PWM mode
	timer_pause(TIMER1);									// Temporarily stop the timer while configuring
	timer_set_count(TIMER1, 0);						// Reset the timer count to 0
	timer_set_reload(TIMER1, period);				// Reload, Compare, and Prescaler values set to 
	timer_set_compare(TIMER1, 1, duty);				// produce a frequency of 32 kHz
	timer_set_prescaler(TIMER1, 1);
	//TIMER1->regs->CR2 

	adc_calibrate(ADC1);
	adc_set_sample_rate(ADC1, ADC_SMPR_7_5);
	adc_set_exttrig(ADC1, 1);
	adc_set_extsel(ADC1, ADC_ADC12_TIM1_CC1);
	adc_set_reg_seqlen(ADC1, 1);

	uint8 ADC1_Sequence[]={1,0,0,0,0,0};
	ADC1->regs->SQR3 |= calc_adc_sequence(ADC1_Sequence);

	adc_enable(ADC1);
	timer_resume(TIMER1);
}

void loop() {
	if((ADC1->regs->SR & 1<<1) == 2)
	{		
		ADC1->regs->SR ^= 1<<1;
		count++;
		if(count==32000)
		{
			count = 0;
			digitalWrite(BOARD_LED_PIN, toggle);
			toggle ^= 1;
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
