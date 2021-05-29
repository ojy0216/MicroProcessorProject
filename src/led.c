 //-------| src/led.c |-------//
#include "led.h" 
static short * led; 

short led_decimal[] = {
    0xFF, 	// 0
	0x7F,	// 1
	0x3F, 	// 2
	0x1F, 	// 3
	0x0F,	// 4
	0x07,	// 5
	0x03, 	// 6
	0x01, 	// 7
	0x00, 	// 8
};

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

void led_write(int decimal){
	*led = led_decimal[decimal];
	usleep(0);
}

void led_blink(){
	for(int i = 0; i < 5; i++){
		led_all();
		usleep(200000);
		led_clear();
		usleep(200000);
	}
}
