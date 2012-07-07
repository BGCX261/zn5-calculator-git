#ifndef DEBUG_H_
#define DEBUG_H_

#include <stdio.h>

#define debug_on 1
#define debug(msg...) if (debug_on) { printf( "zn5Cal: " msg); }

#endif /* DEBUG_H_ */

