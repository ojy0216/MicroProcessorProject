//-------| src/fnd.c |-------//
#include "fnd.h"
#include <stdlib.h>
#include <string.h>

static unsigned short fnd_decimal[10] = {
	0x3F, // 0
	0x06, // 1
	0x5B, // 2
	0x4F, // 3
	0x66, // 4
	0x6D, // 5
	0x7D, // 6
	0x07, // 7
	0x7F, // 8
	0x67, // 9
};

static unsigned short fnd_minus = 0x40;

static short * fnd[MAX_FND];

void init_fnd(short * address[]) {
	int i;
	for( i=0; i<MAX_FND; i++ ) {
		fnd[i] = address[i];
	}
}

void fnd_clear() {
	int i;
	for(i=0; i<MAX_FND; i++){
		*fnd[i] = 0;
	}
}

void fnd_all() {
	int i;
	for(i=0; i<MAX_FND; i++){
		*fnd[i] = 0xFF;
	}
}

void fnd_write(int decimal, int fnd_num) {
	*fnd[fnd_num] = fnd_decimal[decimal];
}

void fnd_number(int number){
	// Receive a number and show at fnd
	// if number < 0, show minus (-) at front
	int abs_number = number > 0 ? number : (number * -1);

	int digit = 0;
	int div = 1;
	while(abs_number / div){
		digit += 1;
		div *= 10;
	}

	int* numArr = malloc(sizeof(int) * digit);

	div = 1;
	for(int i = 0; i < digit; i++){
		numArr[i] = abs_number / div;
		while(numArr[i] >= 10)
			numArr[i] %= 10;
		div *= 10;
	}

	for(int i = 0; i < digit; i++)
		fnd_write(numArr[i], i);

	if(number < 0)
		*fnd[digit] = fnd_minus;

	free(numArr);

	usleep(0);
}
