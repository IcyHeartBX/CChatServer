#include "update_password.h"

/// <summary>
///
/// </summary>
/// <param name="receive_message">example: UpdatePassword{account:10102,password:123,newPassword:ccc}</param>
/// <returns>dest sockfd</returns>
int update_password(const GString* const receive_message,
			GString* const send_message,
			const int sockfd)
{
	gchar* account = get_node(receive_message->str, "(?<=account:)[^,\n}]*");
	gchar* password = get_node(receive_message->str, "(?<=password:)[^,\n}]*");

	char cmd_text[100] = {0,};
	snprintf(cmd_text, sizeof(cmd_text), "call proc_login('%s','%s');", account, password);
	GString* data = g_string_new("");
	get_data(cmd_text, data);
	if (2 == data->len)
	{
		g_string_append(send_message, "UpdatePassword:{'Result':'LoginFail'}");
	}
	else
	{
		char* new_password = get_node(receive_message->str, "(?<=newPassword:)[^,\n}]*");
		sprintf(cmd_text, "call proc_update_password('%s', '%s')", account, new_password);
		operation_data(cmd_text);
		g_string_append(send_message, "UpdatePassword:{'Result':'Success'}");
	}
	g_string_free(data, TRUE);

	return sockfd;
}
