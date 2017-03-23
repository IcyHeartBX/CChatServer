#include "update_userinfo.h"

/// <summary>
///
/// </summary>
/// <param name="receive_message">example: UpdateUserInfo:{account:10102,nickname:ddd,email:ddd@163.com}</param>
/// <returns>dest sockfd</returns>
int update_userinfo(const GString* const receive_message,
			GString* const send_message,
			const int sockfd)
{
	gchar* account = get_node(receive_message->str, "(?<=account:)[^,\n}]*");
	gchar* nickname = get_node(receive_message->str, "(?<=nickname:)[^,\n}]*");
	gchar* email = get_node(receive_message->str, "(?<=email:)[^,\n}]*");

	char cmd_text[100] = {0, };
	snprintf(cmd_text, sizeof(cmd_text), "call proc_update_userinfo('%s', '%s', '%s')", account, nickname, email);
	operation_data(cmd_text);
	g_string_append(send_message, "UpdateUserInfo:{'Result':'Success'}");

	return sockfd;
}
