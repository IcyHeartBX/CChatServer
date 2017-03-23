#ifndef __mysql_helper__
#define __mysql_helper__

#include "common.h"

void open_db_connection(const char* const,
					const char* const,
					const char* const,
					const char* const);
void close_db_connection(void);
void get_data(const char* const, GString* const);
void operation_data(const char* const);

#endif
