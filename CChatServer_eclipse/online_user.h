#ifndef __online_user__
#define __online_user__

#include "common.h"

typedef struct
{
    unsigned long hash_code_one;
    unsigned long hash_code_two;
    char exists;
    char *sockfd;
    char *account;
    char *nickname;
	char *email;
}UserInfo;

void get_online_userlist(GString* const);
void add_online_user(const UserInfo* const);
void remove_online_user(const char* const);
int get_online_users_changed(void);
void set_online_users_changed(int);
void update_all_client_userlist(void);
int user_is_online(const char* const);
int hash_table_insert(char *key, UserInfo *hash_table, int hash_table_count);
int hash_table_remove(char *key, UserInfo *hash_table, int hash_table_count);
void empty_hash_value(UserInfo *the_user_info);
void empty_hash_table(UserInfo *hash_table, int hash_table_count);
void prepare_crypt_table(void);
unsigned long hash_string( char *lpszFileName, unsigned long hash_type);
int get_hash_index(char *the_string, UserInfo *lptable, int ntableSize);
void print_crypt_table(void);
void print_hash_table(UserInfo *hash_table, int table_length);
void print_hash_value(UserInfo *the_user_info);
void init_userlist(void);


#endif
