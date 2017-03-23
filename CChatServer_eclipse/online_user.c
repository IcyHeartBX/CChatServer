#include "online_user.h"
// max online user count
#define MAXONLINECOUNT 10000

static UserInfo userlist_by_account[MAXONLINECOUNT];
static UserInfo userlist_by_sockfd[MAXONLINECOUNT];
static int is_init = 0;
static int userlist_changed = 0;
static unsigned long crypt_table[0x500];

int hash_table_insert(char *key, UserInfo *hash_table, int hash_table_count)
{
	int index = get_hash_index(key, hash_table, hash_table_count);
	if (-1 == index)
	{
		index = hash_string(key, 0) % hash_table_count;
		UserInfo* user = &hash_table[index];
		user->hash_code_one = hash_string(key, 1);
		user->hash_code_two = hash_string(key, 2);
		user->exists = 1;
	}

	return index;
}


//return index
int hash_table_remove(char *key, UserInfo *hash_table, int hash_table_count)
{
	int index = get_hash_index(key, hash_table, hash_table_count);
	if (index != -1)
	{
		empty_hash_value(&hash_table[index]);
	}

	return index;
}


void empty_hash_value(UserInfo *the_user_info)
{
	the_user_info->hash_code_one = 0;
	the_user_info->hash_code_two = 0;
	the_user_info->exists = 0;
}

void empty_hash_table(UserInfo *hash_table, int hash_table_count)
{
	int i;
	for (i = 0; i < hash_table_count; i++)
	{
		empty_hash_value(&hash_table[i]);
	}
}

void prepare_crypt_table(void)
{ 
	unsigned long seed = 0x00100001, index1 = 0, index2 = 0, i;

	for(index1 = 0; index1 < 0x100; index1++ ) 
    { 
		for( index2 = index1, i = 0; i < 5; i++, index2 += 0x100 ) 
        { 
            unsigned long temp1, temp2;
            seed = (seed * 125 + 3) % 0x2AAAAB; 
            temp1 = (seed & 0xFFFF) << 0x10;
            seed = (seed * 125 + 3) % 0x2AAAAB; 
            temp2 = (seed & 0xFFFF);
            crypt_table[index2] = ( temp1 | temp2 ); 
        } 
    } 
}

unsigned long hash_string( char *lpszFileName, unsigned long hash_type) 
{ 
    unsigned char *key  = (unsigned char *)lpszFileName; 
 	unsigned long seed1 = 0x7FED7FED; 

	unsigned long seed2 = 0xEEEEEEEE; 
	int ch;

    while( *key != 0 ) 
    { 
        ch = toupper(*key++);
        seed1 = crypt_table[(hash_type << 8) + ch] ^ (seed1 + seed2); 
        seed2 = ch + seed1 + seed2 + (seed2 << 5) + 3; 
    } 
    return seed1; 
}

//-1: not exists
int get_hash_index(char *the_string, UserInfo *lptable, int ntableSize)
{
 	const int HASH_OFFSET = 0, HASH_A = 1, HASH_B = 2;
	unsigned long nhash_ = hash_string(the_string, HASH_OFFSET);

	unsigned long hash_code_one = hash_string(the_string, HASH_A);
	unsigned long hash_code_two = hash_string(the_string, HASH_B);
	int nhash_Start = nhash_ % ntableSize, nhash_Pos = nhash_Start;

	int index = -1;
	while (lptable[nhash_Pos].exists)
	{
		if (lptable[nhash_Pos].hash_code_one == hash_code_one && lptable[nhash_Pos].hash_code_two == hash_code_two)
		{
			index = nhash_Pos;
		}
		else
		{
			nhash_Pos = (nhash_Pos + 1) % ntableSize;
		}
		if (nhash_Pos == nhash_Start)
		{
			break;
		}
	}
	return index; 
}

void print_crypt_table()
{
    int i = 0;
	for ( ; i < 0x500; i++ ) 
	{ 
		if ( i % 10 == 0 ) 
		{ 
			printf("\n"); 
		}
		printf("%-12lx", crypt_table[i] ); 
	}
}

void print_hash_table(UserInfo *hash_table, int table_length)
{
 	int i;
	for (i = 0; i < table_length; i++)
	{
		if (hash_table[i].exists)
		{
			print_hash_value(&hash_table[i]);
		}
	}
 }

void print_hash_value(UserInfo *the_user_info)
{
 	printf("%-12lx %-12lx %c %-12s %-12s %-12d\n",
		the_user_info->hash_code_one,
		the_user_info->hash_code_two,
		the_user_info->exists,
		the_user_info->account,
		the_user_info->nickname,
		the_user_info->sockfd);
}


/*------------------not hash table realization bellow:-----------------------*/
void init_userlist(void)
{
	if (0 == is_init)
	{
		empty_hash_table(userlist_by_account, MAXONLINECOUNT);
		empty_hash_table(userlist_by_sockfd, MAXONLINECOUNT);
		prepare_crypt_table();
		is_init = 1;
	}
}


int get_online_users_changed(void)
{
	return userlist_changed;
}

void set_online_users_changed(int changed)
{
	userlist_changed = changed;
}


void get_online_userlist(GString* const online_userlist)
{
	init_userlist();

	int i = 0;
	g_string_append(online_userlist, "[");
	for (i = 0; i < MAXONLINECOUNT; i++)
	{
		if (userlist_by_account[i].exists)
		{
			if (1 < online_userlist->len)
			{
				g_string_append(online_userlist, ",");
			}
			char userinfo[120];
			sprintf(userinfo, "{'sockfd':'%s','account':'%s','nickname':'%s','email':'%s'}",
				userlist_by_account[i].sockfd, userlist_by_account[i].account,
				userlist_by_account[i].nickname, userlist_by_account[i].email);
			g_string_append(online_userlist, userinfo);
		}
	}
	g_string_append(online_userlist, "]");
}

void add_online_user(const UserInfo* const userinfo)
{
	init_userlist();
	if(-1 == get_hash_index(userinfo->account, userlist_by_account, MAXONLINECOUNT))
	{
		int user_index = hash_table_insert(userinfo->account, userlist_by_account, MAXONLINECOUNT);
		UserInfo* user = &userlist_by_account[user_index];
		user->sockfd = userinfo->sockfd;
		user->nickname = userinfo->nickname;
		user->account = userinfo->account;
		user->email = userinfo->email;

		user_index = hash_table_insert(userinfo->sockfd, userlist_by_sockfd, MAXONLINECOUNT);
		user = &userlist_by_sockfd[user_index];
		user->sockfd = userinfo->sockfd;
		user->nickname = userinfo->nickname;
		user->account = userinfo->account;
		user->email = userinfo->email;
		set_online_users_changed(1);
	}
}

void remove_online_user(const char* const sockfd)
{
	init_userlist();
	int index = hash_table_remove(sockfd, userlist_by_sockfd, MAXONLINECOUNT);
	if (index > -1)
	{
		hash_table_remove(userlist_by_sockfd[index].account, userlist_by_account,
						MAXONLINECOUNT);
		set_online_users_changed(1);
	}
}

//1: true, 0: false
int user_is_online(const char* const account)
{
	init_userlist();
	
	return (-1 < get_hash_index(account, userlist_by_account, MAXONLINECOUNT));
}

void update_all_client_userlist(void)
{
	init_userlist();

	GString* users = g_string_new("");
	get_online_userlist(users);

	int i = 0;
	for (i = 0; i < MAXONLINECOUNT; i++)
	{
		if (userlist_by_account[i].exists)
		{
			printf("Send userlist:%s, to sockfd:%s\n\n", users->str, userlist_by_account[i].sockfd);
			send(atoi(userlist_by_account[i].sockfd), users->str, users->len, 0);
		}
	}

	g_string_free(users, TRUE);
}
