#include "forgot_password.h"

int handle_forgot_password(const GString* const receive_message,
			GString* const send_message,
			const int sockfd)
{
	//receive_message example: ForgotPassword:{SecurityEmail:abc@gmail.com}
	gchar* security_email = get_node(receive_message->str, "(?<=SecurityEmail:)[^,\n}]*");

	char cmd_text[100] = {0,};
	snprintf(cmd_text, sizeof(cmd_text), "call proc_check_security_email('%s')", security_email);
	GString* data = g_string_new("");
	get_data(cmd_text, data);
	if (2 == data->len)
	{
		g_string_append(send_message, "{'Result':'EmailError'}");
	}
	else
	{
		g_string_append(send_message, "{'Result':'Success'}");
	}
	g_string_free(data, TRUE);

	return sockfd;
}
