#include "mysql_helper.h"
#include <glib.h>

static MYSQL *connection;

void open_db_connection(const char* const host, const char* const user,
		const char* const password, const char* const database)
{
	connection = mysql_init(NULL);
	if (!mysql_real_connect(connection, host, user, password, database, 3306,
			NULL, CLIENT_MULTI_STATEMENTS))
	{
		fprintf(stderr, "connect mysql error: %s\n", mysql_error(connection));
		exit(0);
	}
	printf("mysql database connect successful!\n");
}

void close_db_connection(void)
{
	mysql_close(connection);
	printf("mysql database closed!\n");
}

void get_data(const char* const cmd_text, GString* const data)
{
	MYSQL_RES *res;
	MYSQL_ROW row;
	if (mysql_query(connection, cmd_text))
	{
		fprintf(stderr, "query error:%s\n", mysql_error(connection));
		//exit(0);
	}
	res = mysql_use_result(connection);
	unsigned int num_fields = mysql_num_fields(res);
	MYSQL_FIELD *fields = mysql_fetch_fields(res);
	int i = 0;
	g_string_append(data, "[");
	while ((row = mysql_fetch_row(res)) != NULL)
	{
		if (i > 0)
		{
			g_string_append(data, ",");
		}
		g_string_append(data, "{");
		for (i = 0; i < num_fields; i++)
		{
			if (i > 0)
			{
				g_string_append(data, ",");
			}
//			g_string_append(data, "'");
			g_string_append(data, fields[i].name);
//			g_string_append(data, "':'");
			g_string_append(data, ":");
			g_string_append(data, row[i] ? row[i] : "NULL");
//			g_string_append(data, "'");
		}
		g_string_append(data, "}");
	}
	g_string_append(data, "]");

	while (!mysql_next_result(connection))
		;
}

void operation_data(const char* const cmd_text)
{
	MYSQL_RES *res;
	if (mysql_query(connection, cmd_text))
	{
		fprintf(stderr, "operation data error:%s\n", mysql_error(connection));
		//exit(0);
	}
}
