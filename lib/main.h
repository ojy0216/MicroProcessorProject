//-------| lib/main.h |-------//
#ifndef	MAIN_H
#define	MAIN_H 


#include <stdio.h>		// printf(), scanf(), fprintf()
#include <fcntl.h>		// open()
#include <unistd.h>		// close(), off_t , usleep()
#include <sys/mman.h>	// mmap(), munmap()
#include <stdlib.h> 	// exit()
#include <string.h>		// strlen()

#include <sys/time.h>
#include <time.h>
#include <sys/wait.h>

#include "ieb.h"
#include "led.h"
#include "dot.h"
#include "fnd.h"
#include "clcd.h"

#define S 0

typedef enum {
	FALSE = 0,
	TRUE  = 1
} truth_t;

typedef enum {
	ERROR = -1,
	SUCCESS = 0
} error_t;

// extra
typedef struct timeval time_st;
#define SEC_TO_US 1000000.0

enum {
	PASS, FAIL
};

double getTimeDiff(time_st start, time_st end);

int main(int argc, char* argv[]);
short * mapper(off_t offset);
void unmapper();
void emergency_closer();

truth_t inputter();

void sim_init();

void rhythm_game();
void reaction_game();
void memory_game();
void baseball_game();

void signalControl();

#endif
