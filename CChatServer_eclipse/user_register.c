#include "user_register.h"

/// <summary>
/// 
/// </summary>
/// <param name="receive_message">example:
/// UserRegister:{nickname:abc,password:123,email:abc@163.com,SecurityEmail:abc@gmail.com}
/// </param>
/// <returns></returns>
int handle_user_register(const GString* const receive_message, GString* const send_message, const int sockfd)
{
	gchar* nickname = get_node(receive_message->str, "(?<=nickname:)[^,\n}]*");
	gchar* password = get_node(receive_message->str, "(?<=password:)[^,\n}]*");
	gchar* email = get_node(receive_message->str, "(?<=email:)[^,\n}]*");
	gchar* security_email = get_node(receive_message->str, "(?<=SecurityEmail:)[^,\n}]*");
	GString* data = g_string_new("");
	get_data("call proc_get_new_account()", data);
	gchar* account = get_node(data->str, "(?<=account:)[^,\n}]*");
	char cmd_text[100] = {0,};
	snprintf(cmd_text, sizeof(cmd_text), "call proc_register_user('%s','%s','%s','%s','%s','%s')",
		account, password, nickname, email, security_email, "127.0.0.1");
//	printf("cmd_test: %s\n", cmd_text);
	operation_data(cmd_text);
	snprintf(cmd_text, sizeof(cmd_text), "{'nickname':'%s','account':'%s','sockfd':'%d'}", nickname, account, sockfd);
	g_string_append(send_message, cmd_text);

	g_string_free(data, TRUE);
	
	return sockfd;
}
