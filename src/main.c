//-------| src/main.c |-------//
#include "main.h"

static off_t IEB_DOT[MAX_DOT] = {
	IEB_DOT1,
	IEB_DOT2,
	IEB_DOT3,
	IEB_DOT4,
	IEB_DOT5
};
static off_t IEB_FND[MAX_FND] = {
	IEB_FND0,
	IEB_FND1,
	IEB_FND2,
	IEB_FND3,
	IEB_FND4,
	IEB_FND5,
	IEB_FND6,
	IEB_FND7
};


static int fd;
static int map_counter = 0;
static void * map_data[100];


int main(int argc, char* argv[]) {

	int i;
	short * led, * dot[MAX_DOT], * fnd[MAX_FND], *clcd_cmd, *clcd_data;

	fd = open("/dev/mem", O_RDWR|O_SYNC);
	if (fd == -1) {
		fprintf(stderr, "Cannot open /dev/mem file");
		exit(EXIT_FAILURE);
	}

	led = mapper(IEB_LED);
	for( i=0; i<MAX_DOT; i++ ) {
		dot[i] = mapper(IEB_DOT[i]);
	}
	for( i=0; i<MAX_FND; i++ ) {
		fnd[i] = mapper(IEB_FND[i]);
	}

	clcd_cmd = mapper(IEB_CLCD_CMD);
	clcd_data = mapper(IEB_CLCD_DATA);

	init_led(led);
	init_dot(dot);
	init_fnd(fnd);
	init_clcd(clcd_cmd, clcd_data);

	clcd_entry_mode_set(1, S);

	while(inputter() == TRUE);

	unmapper();
	close(fd);
	return 0;
}

short * mapper(off_t offset) {
	map_data[map_counter] = mmap(NULL, sizeof(short), PROT_WRITE, MAP_SHARED, fd, offset);
	if ( map_data[map_counter] == MAP_FAILED ) {
		fprintf(stderr, "Cannot do mmap()");
		emergency_closer();
	}
	return (short *)map_data[map_counter++];
}

void unmapper() {
	int i;
	for( i=0; i<map_counter; i++) {
		munmap(map_data[i], sizeof(short));
	}
	map_counter = 0;
}

void emergency_closer() {
	unmapper();
	close(fd);
	exit(EXIT_FAILURE);
}

double getTimeDiff(time_st start, time_st end){
    // Interval in seconds
    return (double)((end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / SEC_TO_US);
}

void rhythm_game(){
	clcd_set_DDRAM(2);
	clcd_write_string("Rhythm Game");

	time_st start, end;

	while(getchar() != '\n');

    double interval = (double)(rand() % 10 + 5) / 10;   // 0.5 ~ 1.5 s, 0.1s interval
    int interval_us = (int)(interval * SEC_TO_US);

	fnd_clear();
	dot_countdown();

	led_example_rhythm(interval_us);
	gettimeofday(&start, NULL);

	getc(stdin);		//enter detect
	gettimeofday(&end, NULL);		

	int diff = (int)((1000 * (getTimeDiff(start, end) - interval)));	// sec -> ms

	fnd_number(diff);
}

truth_t inputter() {
	int input_int;
	printf("\n");
	printf("***************************************\n");
	printf("*     Please type a number            *\n");
	printf("*     1 : Rhythm Game                 *\n");
	printf("*     2 : Reaction Game               *\n");
	printf("*     3 : Memory Game                 *\n");
	printf("*     4 : Baseball Game               *\n");
	printf("*     0 : EXIT                        *\n");
	printf("***************************************\n\n");
	scanf("%d", &input_int);
	
	switch(input_int) {
		case 1 :
			rhythm_game();
			break;
		case 2 :
			break;
		case 3 :
			break;
		case 4 :
			break;
		case 0 :
			return FALSE;
		default :
			break;
	}
	return TRUE;
}