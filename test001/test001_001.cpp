/*
*
————————————————
版权声明：本文为CSDN博主「百恼」的原创文章，遵循CC 4.0 BY - SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https ://blog.csdn.net/zsy19881226/article/details/65442768
*/

#include <windows.h>

#include <cassert>
#include <cstdio>
#include <ctime>
//#include <nanomsg/nn.h>
//#include <nanomsg/pubsub.h>
#include <nng/nng.h>
#include <nng/compat/nanomsg/nn.h>
#include <nng/compat/nanomsg/pubsub.h>
#include <iostream>

//#pragma comment(lib, "nanomsg.lib")

#define SERVER "server"
#define CLIENT "client"

char* date()
{
    time_t raw = time(&raw);
    struct tm* info = localtime(&raw);
    char* text = asctime(info);
    text[strlen(text) - 1] = '\0'; // remove '\n'
    return text;
}

int server(const char* url)
{
    int sock = nn_socket(AF_SP, NN_PUB);
    assert(sock >= 0);
    assert(nn_bind(sock, url) >= 0);
    while (1)
    {
        char* d = date();
        int sz_d = strlen(d) + 1; // '\0' too
        printf_s("SERVER: PUBLISHING DATE %s\n", d);
        int bytes = nn_send(sock, d, sz_d, 0);
        assert(bytes == sz_d);
        Sleep(1000); // 1 s
    }
    return nn_shutdown(sock, 0);
}

int client(const char* url, const char* name)
{
    int sock = nn_socket(AF_SP, NN_SUB);
    assert(sock >= 0);
    // TODO learn more about publishing/subscribe keys
    assert(nn_setsockopt(sock, NN_SUB, NN_SUB_SUBSCRIBE, "", 0) >= 0);
    assert(nn_connect(sock, url) >= 0);
    while (1)
    {
        char* buf = NULL;
        int bytes = nn_recv(sock, &buf, NN_MSG, 0);
        assert(bytes >= 0);
        printf_s("CLIENT (%s): RECEIVED %s\n", name, buf);
        nn_freemsg(buf);
    }
    return nn_shutdown(sock, 0);
}

int main(int argc, char** argv)
{
    printf_s("Hello, insigmaudpserver...\n");

    printf_s("argc: %d\n", argc);

    if (strncmp(SERVER, argv[1], strlen(SERVER)) == 0 && argc >= 2)
        return server(argv[2]);
    else if (strncmp(CLIENT, argv[1], strlen(CLIENT)) == 0 && argc >= 3)
        return client(argv[2], argv[3]);
    else
    {
        fprintf(stderr, "Usage: pubsub %s|%s <URL> <ARG> ...\n",
            SERVER, CLIENT);
        return 1;
    }
    //return 0;
}
