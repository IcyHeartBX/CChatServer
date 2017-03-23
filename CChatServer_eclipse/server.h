#ifndef __io_server__
#define __io_server__

#include "program.h"

//int set_nonblocking(int);
static int handle_message(int);
void server_start(void);
void close_server(void);

#endif
