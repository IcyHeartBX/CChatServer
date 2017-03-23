#include "refresh_userlist.h"

int refresh_userlist(const GString* const receive_message, GString* const send_message, const int sockfd)
{
	get_online_userlist(send_message);

	return sockfd;
}
