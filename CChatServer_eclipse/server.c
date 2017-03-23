#include "server.h"

#define MAXBUF 10240
#define MAXEPOLLSIZE 10000

static int _listener;


static int set_nonblocking(int sockfd)
{
    if (fcntl(sockfd, F_SETFL, fcntl(sockfd, F_GETFD, 0)|O_NONBLOCK) == -1)
    {
        return -1;
    }
    return 0;
}


static void set_keep_alive(void)
{
	int   keepAlive   =   1;
	int   keepIdle   =   5;
	int   keepInterval   =   5;
	int   keepCount   =   3;

	if(setsockopt(_listener, SOL_SOCKET,SO_KEEPALIVE,(void*)&keepAlive,sizeof(keepAlive))   ==   -1)  
    {
		printf("so_keepalive error: %d, message: %s\n", errno, strerror(errno));
		exit(1);
	}  

	if(setsockopt(_listener, SOL_TCP,TCP_KEEPIDLE,(void   *)&keepIdle,sizeof(keepIdle))   ==   -1)  
	{
		printf("tcp_keepidle error: %d, message: %s\n", errno, strerror(errno));
		exit(1);
	}  

	if(setsockopt(_listener, SOL_TCP,TCP_KEEPINTVL,(void   *)&keepInterval,sizeof(keepInterval))   ==   -1)  
	{  
		printf("tcp_keepintvl error: %d, message: %s\n", errno, strerror(errno));
		exit(1);
	}  

	if(setsockopt(_listener, SOL_TCP,TCP_KEEPCNT,(void   *)&keepCount,sizeof(keepCount))   ==   -1)  
	{
		printf("tcp_keepcnt error: %d, message: %s\n", errno, strerror(errno));
		exit(1);
	}  
}

//summary
static int handle_message(int sockfd)
{
    char buf[MAXBUF + 1];
    int len, dest_sockfd = 0;
    
    bzero(buf, MAXBUF + 1);
    len = recv(sockfd, buf, MAXBUF, 0);
	snprintf(buf, sizeof(buf), "%s,{Sockfd:%d}", buf, sockfd);
		GString* receive_message = g_string_new(buf);
	set_online_users_changed(0);
    if (len > 0)
    {
        printf("Message received! Sockfd: %d, Message: %s, Length: %d\n", sockfd, buf, strlen(buf));
//		Login:{account:382939322,password:abc}		
		GString* send_message = g_string_new("");
		int dest_sockfd = handler(receive_message, send_message, sockfd);
		printf("send message: %s\n", send_message->str);
		if (-1 == send(dest_sockfd, send_message->str, send_message->len, 0))
		{
			if (9 == errno) // dest_sockfd not exists
			{
				char* hint_message = "{'Result':'SendMessageFail'}";
				send(sockfd, hint_message, strlen(hint_message), 0);
			}
		}

		g_string_free(send_message, TRUE);
    }
    else
    {
        if (len < 0)
        {
            printf("Message receive fail! Sockfd: %d, ErrorCode: %d, ErrorInfo: %s\n",
				sockfd, errno, strerror(errno));
        }
        else
        {
            printf("Connection disconnect! Sockfd: %d\n", sockfd);
        }
		printf("Close sockfd: %d\n", sockfd);
        close(sockfd);
		remove_online_user(get_node(receive_message->str, "(?<=Sockfd:)[^,\n}]*"));
		len = -1;
    }
	g_string_free(receive_message, TRUE);
	if (get_online_users_changed())
	{
		update_all_client_userlist();	
	}

    return len;
}


void server_start(void)
{
	int listener, new_fd, kdpfd, nfds, n, ret, curfds;
    socklen_t len;
    struct sockaddr_in my_addr, their_addr;
    unsigned int myport = 1234, lisnum = 1;
    struct epoll_event ev;
    struct epoll_event events[MAXEPOLLSIZE];
    struct rlimit rt;

    rt.rlim_max = rt.rlim_cur = MAXEPOLLSIZE;
    if (setrlimit(RLIMIT_NOFILE, &rt) == -1)
    {
        perror("setrlimit");
        exit(1);
    }
    else
    {
        printf("System paramters setting successful!\n");
    }

    if ((listener = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket");
        exit(1);
    }
    else
    {
        printf("Socket create successful!\n");
    }


	_listener = listener;
    set_nonblocking(listener);

    bzero(&my_addr, sizeof(my_addr));
    my_addr.sin_family = PF_INET;
    my_addr.sin_port = htons(myport);
    my_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listener, (struct sockaddr *) &my_addr, sizeof(struct sockaddr)) == -1)
    {
        perror("bind");
        exit(1);
    }
    else
    {
        printf("IP address and port bind successful!\n");
    }

    if (listen(listener, lisnum) == -1)
    {
        perror("listen");
        exit(1);
    }
    else
    {
        printf("Startup service successful!\n");
    }

    kdpfd = epoll_create(MAXEPOLLSIZE);
    len = sizeof(struct sockaddr_in);
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = listener;
    if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, listener, &ev) < 0)
    {
        fprintf(stderr, "epoll set insertion error: fd=%d\n", listener);
		exit(1);
    }
    else
    {
        printf("Add listen socket to epoll successfull!\n");
    }

	set_keep_alive();

    curfds = 1;
    while (1)
    {
        nfds = epoll_wait(kdpfd, events, curfds, -1);
        if (nfds == -1)
        {
            perror("epoll_wait");
            break;
        }
        
        for (n = 0; n < nfds; ++n)
        {
            if (events[n].data.fd == listener)
            {
                new_fd = accept(listener, (struct sockaddr *) &their_addr, &len);
                if (new_fd < 0)
                {
                    perror("accept");
                    continue;
                }
                else
                {
                    printf("\nConnection from to: %s:%d, Sockfd:%d\n",
						inet_ntoa(their_addr.sin_addr), ntohs(their_addr.sin_port), new_fd);
                }

                set_nonblocking(new_fd);
                ev.events = EPOLLIN | EPOLLET;
                ev.data.fd = new_fd;
                if (epoll_ctl(kdpfd, EPOLL_CTL_ADD, new_fd, &ev) < 0)
                {
                    fprintf(stderr, "Add socket to epoll fail! Sockfd: %d, ErrorInfo: %s\n", new_fd, strerror(errno));
					//exit(1);
                }
				else
				{
	                curfds++;
				}
            }
            else
            {
                ret = handle_message(events[n].data.fd);
                if (ret < 1 && errno != 11)
                {
                    epoll_ctl(kdpfd, EPOLL_CTL_DEL, events[n].data.fd, &ev);
                    curfds--;
                }
            }
        }
    }
}


void close_server(void)
{
	close(_listener);
}
