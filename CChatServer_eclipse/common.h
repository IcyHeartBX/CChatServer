#ifndef COMMON_H
#define COMMON_H

/* system header files */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>
#include <arpa/inet.h>
//#include <openssl/ssl.h>
//#include <openssl/err.h>
#include <fcntl.h>
#include <sys/epoll.h>
#include <netinet/tcp.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <mysql/mysql.h>
#include <glib.h>

/* third party header files */


typedef struct{
	char *host;
	char *user;
	char *passwd;
	char *db;
}_mysql;

typedef struct {
	_mysql mysql;
}Configure;

#endif
