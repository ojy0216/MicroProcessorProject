//-------| lib/dot.h |-------//
 #ifndef DOT_H 
#define DOT_H 
#include <unistd.h> // usleep() 
 #define MAX_DOT 5 

 #define DOT_SEC_TO_US 1000000

 enum {
     CORRECT, WRONG
 };

 enum {
     SMILE, SAD
 };

 void init_dot(short * address[]); 
 void dot_clear(); 
 void dot_write(int decimal); 

 void dot_countdown();
 void dot_show_ox(int result);
 void dot_show_face(int result);
 void dot_show_question();
 #endif
