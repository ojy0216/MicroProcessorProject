 //-------| lib/led.h |-------// 
 #ifndef LED_H 
 #define LED_H 
 #include <unistd.h> // usleep() 
 #define MAX_LED 8
 void init_led(short * address); 
 void led_clear();
 void led_all();
 void led_example_rhythm(int interval);
 void led_write(int decimal);
 void led_blink();
 #endif
