// Stan Okrasinski     10/11/11
// TEST program, sets up PWM signal of 32kHz
// using the timer pheripherals. Signal output 
// from Timer 2, channel 1 (pin D48)

#include "wirish.h"
#include "timer.h"

void setup() {
   pinMode(23, PWM);
	pinMode(48, PWM);
	pinMode(54, PWM);
	pinMode(38, PWM);

   timer_set_mode(TIMER1, 1, TIMER_PWM);
	timer_pause(TIMER1);
	timer_set_count(TIMER1, 0);
	timer_set_reload(TIMER1, 1125);
	timer_set_compare(TIMER1, 1, 562);
	timer_set_prescaler(TIMER1, 1);
	
	timer_pause(TIMER5);	
	timer_disable(TIMER5);

	timer_set_mode(TIMER2, 1, TIMER_PWM);
	timer_pause(TIMER2);
	timer_set_count(TIMER2, 0);
	timer_set_reload(TIMER2, 1125);
	timer_set_compare(TIMER2, 1, 562);
	timer_set_prescaler(TIMER2, 3);

	timer_set_mode(TIMER3, 1, TIMER_PWM);
	timer_pause(TIMER3);
	timer_set_count(TIMER3, 0);
	timer_set_reload(TIMER3, 1125);
	timer_set_compare(TIMER3, 1, 562);
	timer_set_prescaler(TIMER3, 7);

	timer_set_mode(TIMER4, 1, TIMER_PWM);
	timer_pause(TIMER4);
	timer_set_count(TIMER4, 0);
	timer_set_reload(TIMER4, 1125);
	timer_set_compare(TIMER4, 1, 562);
	timer_set_prescaler(TIMER4, 15);
	
	timer_resume(TIMER1);
	timer_resume(TIMER2);
	timer_resume(TIMER3);
	timer_resume(TIMER4);


}

void loop() {

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
