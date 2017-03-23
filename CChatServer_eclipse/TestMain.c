#include <stdio.h>
#include <stdlib.h>
#include "program.h"
#include <glib.h>

void test_cfg()
{
	char content[1000] = {0};
	printf("test file helper\n");
	readfile("res/my.cfg",content,sizeof(content));
	printf("res/my.cfg content:%s\n",content);
}

void test_mysql()
{
	printf("test mysql_helper\n");
	open_db_connection("192.168.24.148", "root", "123456", "yhy");
	print_data();
	close_db_connection();
}

void print_data()
{
	GString *data = g_string_new("");
	//get_data("select * from user_info where 1=3", data);
	get_data("select * from T_UserInfo where id=1", data);
	printf("\n------------------\ndata=%s\n", data->str);
	printf("data length=%lu\n", data->len);
	g_string_free(data, TRUE);
}

void test_regex()
{
	printf("test regex\n");
	char content[1000]="[{id:123,nickname:trueboy}]";//"Login:{account:17895468, password:abc}";

	//	printf("account=%s\n", get_node(content, "(?<=account:)[^,\n}]*"));
	//	printf("password=%s\n", get_node(content, "(?<=password:)[^,\n}]*"));
	printf("id=%s\n", get_node(content, "(?<=id:)[^,\n}]*"));
	printf("nickname=%s\n", get_node(content, "(?<=nickname:)[^,\n}]*"));
	gchar* sockfd;
	sprintf(sockfd, "%d", 99);
	printf("sockfd:%s\n", sockfd);
}

void test_login_a(char* buf, int sockfd)
{
	GString* receive_message = g_string_new(buf);
	GString* send_message = g_string_new("");

	handle_login(receive_message, send_message, sockfd);

	printf("send_message=%s\n", send_message->str);

	g_string_free(send_message, TRUE);
	g_string_free(receive_message, TRUE);
}

void test_login()
{
	printf("test login\n");
	char content[1000];
	readfile("res/my.cfg", content, sizeof(content));
	char* host = get_node(content, "(?<=host:)[^,\n}]*");
	char* user = get_node(content, "(?<=user:)[^,\n}]*");
	char* password = get_node(content, "(?<=password:)[^,\n}]*");
	char* database = get_node(content, "(?<=database:)[^,\n}]*");
	open_db_connection(host, user, password, database);
	test_login_a("Login:{account:10100,password:admin}", 5);
	test_login_a("Login:{account:10101,password:abc}", 6);
	close_db_connection();
}

int test_server(void)
{
	server_start();
	close_server();
	return 0;
}

int main()
{
//	test_cfg();
//	test_mysql();
//	test_regex();
//		test_login();
	test_server();
	printf("hello....\n");
	return 0;
}
