#include "handler.h"

static gboolean is_action(const gchar* const message, const gchar* const action)
{
	return g_str_has_prefix(message, action);
}

int handler(const GString* const receive_message, GString* const send_message, const int sockfd )
{
	int dest_sockfd = sockfd;
	if (TRUE == is_action(receive_message->str, "Login"))
	{
		dest_sockfd = handle_login(receive_message, send_message, sockfd);
	}
	else if (TRUE == is_action(receive_message->str, "SendMessage"))
	{
		dest_sockfd = handle_send_message(receive_message, send_message, sockfd);
	}
	else if (TRUE == is_action(receive_message->str, "ForgotPassword"))
	{
		dest_sockfd = handle_forgot_password(receive_message, send_message, sockfd);
	}
	else if (TRUE == is_action(receive_message->str, "UpdateUserInfo"))
	{
		dest_sockfd = update_userinfo(receive_message, send_message, sockfd);
	}
	else if (TRUE == is_action(receive_message->str, "UpdatePassword"))
	{
		dest_sockfd = update_password(receive_message, send_message, sockfd);
	}
	else if (TRUE == is_action(receive_message->str, "UserRegister"))
	{
		dest_sockfd = handle_user_register(receive_message, send_message, sockfd);
	}
	else if (TRUE == is_action(receive_message->str, "RefreshUserlist"))
	{
		//get_online_userlist(GString* const);
		dest_sockfd = refresh_userlist(receive_message, send_message, sockfd);
	}

	return dest_sockfd;
}
