// Stan Okrasinski     10/22/11
// TEST program, sets up PWM signals on four
// different output channels using the timer
// pheripherals. Signal output from: 
// Timer 1, channel 1 (pin D23): 32 kHz
// Timer 2, channel 1 (pin D48): 16 kHz
// Timer 3, channel 1 (pin D54): 8  kHz
// Timer 4, channel 1 (pin D38): 4  kHz

#include "wirish.h"
#include "timer.h"

void setup() {
   pinMode(23, PWM);										// Timer 1, Channel 1
	pinMode(48, PWM);										// Timer 2, Channel 1
	pinMode(54, PWM);										// Timer 3, Channel 1
	pinMode(38, PWM);										// Timer 4, Channel 1

   timer_set_mode(TIMER1, 1, TIMER_PWM);			// Set up Timer 1, Channel 1 to operate in PWM mode	
	timer_pause(TIMER1);									// Temporarily stop the timer while configuring
	timer_set_count(TIMER1, 0);						// Reset the timer count to 0
	timer_set_reload(TIMER1, 562);					// Reload, Compare, and Prescaler values set to 
	timer_set_compare(TIMER1, 1, 281);				// produce a frequency of 32 kHz
	timer_set_prescaler(TIMER1, 1);
	
	timer_pause(TIMER5);									// Because Timer 5 shares output pin with Timer 2
	timer_disable(TIMER5);								// it is disabled to ensure no conflict

	timer_set_mode(TIMER2, 1, TIMER_PWM);			// Similar configuration to Timer 1
	timer_pause(TIMER2);
	timer_set_count(TIMER2, 0);
	timer_set_reload(TIMER2, 562);
	timer_set_compare(TIMER2, 1, 281);				// Frequency: 16 kHz 
	timer_set_prescaler(TIMER2, 3);

	timer_set_mode(TIMER3, 1, TIMER_PWM);			// Similar configuration to Timer 1
	timer_pause(TIMER3);
	timer_set_count(TIMER3, 0);
	timer_set_reload(TIMER3, 562);
	timer_set_compare(TIMER3, 1, 281);				// Frequency: 8 kHz
	timer_set_prescaler(TIMER3, 7);

	timer_set_mode(TIMER4, 1, TIMER_PWM);			// Similar configuration to Timer 1
	timer_pause(TIMER4);
	timer_set_count(TIMER4, 0);
	timer_set_reload(TIMER4, 562);
	timer_set_compare(TIMER4, 1, 281);				// Frequency: 4 kHz
	timer_set_prescaler(TIMER4, 15);
	
	timer_resume(TIMER1);								// Begin timers
	timer_resume(TIMER2);
	timer_resume(TIMER3);
	timer_resume(TIMER4);


}

void loop() {
// Nothing!
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
