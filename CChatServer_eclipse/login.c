#include "login.h"

int handle_login(const GString* const receive_message,
			GString* const send_message,
			const int sockfd)
{
	//receive_message example: Login:{account:38293698, password:abc}
	//get user' account and password
	gchar* account = get_node(receive_message->str, "(?<=account:)[^,\n}]*");
	gchar* password = get_node(receive_message->str, "(?<=password:)[^,\n}]*");
	//check whether the user is online?
	//if it online,return his sockfd
	if (user_is_online(account))
	{
		g_string_append(send_message, "Logged");
		return sockfd;
	}
	//dont online,continue..
	char cmd_text[100] = {0,};
//	snprintf(cmd_text, sizeof(cmd_text), "call T_UserInfo('%s','%s');", account, password);
	snprintf(cmd_text,sizeof(cmd_text),"SELECT * FROM T_UserInfo WHERE account='%s' AND password = '%s'",account,password);
	GString* data = g_string_new("");
	get_data(cmd_text, data);
	if (2 == data->len)
	{
		g_string_append(send_message, "{'Result':'LoginFail'}");
	}
	else
	{
		gchar* nickname = get_node(data->str, "(?<=nickname:)[^,\n}]*");
		gchar* str_sockfd = get_node(receive_message->str, "(?<=Sockfd:)[^,\n}]*");
		gchar* email = get_node(data->str, "(?<=email:)[^,\n}]*");
		UserInfo userinfo;
		userinfo.sockfd = str_sockfd;
		userinfo.account = account;
		userinfo.nickname = nickname;
		userinfo.email = email;
		add_online_user(&userinfo);
		get_online_userlist(send_message);
	}
	g_string_free(data, TRUE);

	return sockfd;
}
