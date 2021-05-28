#include "dot.h"

static unsigned short dot_decimal[10][MAX_DOT] = {
	{0x7f, 0x41, 0x41, 0x41, 0x7F}, // 0
	{0x00, 0x00, 0x7F, 0x00, 0x00}, // 1
	{0x4f, 0x49, 0x49, 0x49, 0x79}, // 2
	{0x49, 0x49, 0x49, 0x49, 0x7f}, // 3
	{0x78, 0x08, 0x08, 0x7f, 0x08}, // 4
	{0x79, 0x49, 0x49, 0x49, 0x4f}, // 5
	{0x7f, 0x49, 0x49, 0x49, 0x4f}, // 6
	{0x40, 0x40, 0x40, 0x40, 0x7f}, // 7
	{0x7f, 0x49, 0x49, 0x49, 0x7f}, // 8
	{0x78, 0x48, 0x48, 0x48, 0x7f}  // 9
};

static unsigned short dot_ox[][MAX_DOT] = {
	{0x3e, 0x41, 0x41, 0x41, 0x3e},	// O
	{0x22, 0x14, 0x08, 0x14, 0x22}	// X
};

static unsigned short dot_face[][MAX_DOT] = {
	{0x04, 0x22, 0x02, 0x22, 0x04},	// smile
	{0x02, 0x24, 0x04, 0x24, 0x02}	// sad
};

static short * dot[MAX_DOT];

void init_dot(short * address[]) {
	int i;
	for( i=0; i<MAX_DOT; i++ ) {
		dot[i] = address[i];
	}
}

void dot_clear() {
	int i;
	for(i=0; i<MAX_DOT; i++){
		*dot[i] = 0;
	}
	usleep(0); // for Ximulator
}

void dot_write(int decimal) {
	int i;
	for(i=0; i<MAX_DOT; i++){
		*dot[i] = dot_decimal[decimal][i];
	}
	usleep(0); // for Ximulator
}

void dot_countdown(){
	for(int i = 0; i < 3; i++){
		dot_write(3 - i);
		usleep(DOT_SEC_TO_US);
	}
	dot_write(0);
}

void dot_show_ox(int result){
	for(int i = 0; i < MAX_DOT; i++)
		*dot[i] = dot_ox[result][i];
	usleep(0);
}

void dot_show_face(int result){
	for(int i = 0; i < MAX_DOT; i++)
		*dot[i] = dot_face[result][i];
	usleep(0);
}