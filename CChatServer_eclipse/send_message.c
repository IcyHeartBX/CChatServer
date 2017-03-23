#include "send_message.h"

/// <summary>
/// </summary>
/// <param name="receive_message"> example:
/// SendMessage:{account:10102,nickname:abc,message:ssdfasjfs;sdfj,DestSockfd:6}
/// send_message(output param) example:
/// 
/// </param>
/// <returns>dest sockfd</returns>
int handle_send_message(const GString* const receive_message,
						GString* const send_message,
						const int sockfd)
{
	gchar* account = get_node(receive_message->str, "(?<=account:)[^,\n}]*");
	gchar* nickname = get_node(receive_message->str, "(?<=nickname:)[^,\n}]*");
	gchar* dest_sockfd = get_node(receive_message->str, "(?<=DestSockfd:)[^,\n}]*");
	char message[10240] = {0,};
	snprintf(message, sizeof(message), "ReceiveMessage:{'account':'%s','nickname':'%s','message':'%s','sockfd':'%d'}",
		account, nickname, get_node(receive_message->str, "(?<=message:)[^,\n}]*"), sockfd);
	g_string_append(send_message, message);

	return atoi(dest_sockfd);
}
