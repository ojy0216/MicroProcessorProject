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

	srand(time(NULL));

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
	sim_init();

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

void reaction_game(){
	sim_init();

	clcd_set_DDRAM(1);
	clcd_write_string("Reaction Game");

	time_st start, end;

	while(getchar() != '\n');

	int delay = (int)((rand() % 30) * SEC_TO_US / 10 + SEC_TO_US);		// 1 ~ 3 s, 0.1 s interval

	dot_countdown();

	usleep(delay);
	led_all();
	gettimeofday(&start, NULL);

	getc(stdin);		// enter detect
	gettimeofday(&end, NULL);

	int diff = (int)(1000 * getTimeDiff(start, end));

	fnd_number(diff);
}

void memory_game(){
	sim_init();

	clcd_set_DDRAM(2);
	clcd_write_string("Memory Game");

	time_st start, end;

	int genNum;
	int digit = 1;
	int playerGuess;

	int gameResult = PASS;

	for(int i = 0; i < MAX_LED; i++){
		led_write(i + 1);

		genNum = rand() % (digit * 9) + digit;
		fnd_number(genNum);

		usleep(SEC_TO_US);

		fnd_clear();

		scanf("%d", &playerGuess);
		if(genNum == playerGuess){
			dot_show_ox(CORRECT);
			usleep(SEC_TO_US);
			dot_clear();
		}
		else{
			dot_show_ox(WRONG);
			fnd_number(genNum);
			usleep(SEC_TO_US);
			gameResult = FAIL;
			break;
		}

		digit *= 10;
	}

	dot_show_face(gameResult);
}

int check_validity(char* input){
	if(strlen(input) != 4)
		return FALSE;

	for(int i = 0; i < 4; i++){
		for(int j = 0; j < i; j++){
			if(input[i] == input[j])
				return FALSE;
		}
	}

	return TRUE;
}

void baseball_game(){
	sim_init();

	clcd_set_DDRAM(1);
	clcd_write_string("Baseball Game");

	int goal[4];
	char guessStr[5];

	int s, b;

	/* generate goal number*/
	for(int i = 0; i < 4; i++){
		goal[i] = rand() % 10;
		for(int j = 0; j < i; j++){
			if(goal[i] == goal[j]){
				i--;
				continue;
			}
		}
	}

	for(int round = 1; round < 10; round++){
		dot_write(round);

		s = 0;
		b = 0;

		scanf("%s", guessStr);

		if(strcmp(guessStr, "hack") == 0){
			for(int i = 0; i < 4; i++)
				fnd_write(goal[i], 3 - i);

			round--;
			usleep(SEC_TO_US);
			fnd_clear();
			continue;
		}

		if(!check_validity(guessStr)){
			dot_show_ox(FAIL);
			usleep(SEC_TO_US);
			return;
		}

		/* calculate s, b */
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				if(guessStr[i] - '0' == goal[j])
					b++;
			}
			if(guessStr[i] - '0' == goal[i]){
				s++;
				b--;
			}
		}

		fnd_baseball(guessStr, s, b);
		
		if(s == 4){
			dot_show_face(SUCCESS);
			led_blink();
			return;
		}

		usleep(SEC_TO_US);
	}

	dot_show_face(FAIL);
}

void sim_init(){
	clcd_clear_display();
	dot_clear();
	fnd_clear();
	led_clear();
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
			reaction_game();
			break;
		case 3 :
			memory_game();
			break;
		case 4 :
			baseball_game();
			break;
		case 0 :
			return FALSE;
		default :
			break;
	}
	return TRUE;
}