 //-------| src/led.c |-------//
#include "led.h" 
static short * led; 
void init_led(short * address) { 
	led = address; 
 }

void led_clear(){
	*led = (short)~0x00;
}

void led_all(){
	*led = (short)~0xFF;
	usleep(0);
}

void led_example_rhythm(int interval){
	*led = (short)~0x03;
	for(int i = 0; i < 3; i++){
		usleep(interval);
		*led = *led << 2;
	}
	usleep(0);
}
