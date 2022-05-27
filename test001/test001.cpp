﻿/*
https://nanomsg.org/gettingstarted/nng/pubsub.html
./pubsub server ipc:///tmp/pubsub.ipc & server=$! && sleep 1
./pubsub client ipc:///tmp/pubsub.ipc client0 & client0=$!
./pubsub client ipc:///tmp/pubsub.ipc client1 & client1=$!
./pubsub client ipc:///tmp/pubsub.ipc client2 & client2=$!
sleep 5
kill $server $client0 $client1 $client2

windows usage:
pubsub server  "ws://127.0.0.1:7715"
pubsub client  "ws://127.0.0.1:7715"

./test001 server  "ws://127.0.0.1:7715"
./test001 client  "ws://127.0.0.1:7715"
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include <nng/nng.h>
#include <nng/protocol/pubsub0/pub.h>
#include <nng/protocol/pubsub0/sub.h>

#define SERVER "server"
#define CLIENT "client"

#include <windows.h>

void
fatal(const char* func, int rv)
{
    fprintf(stderr, "%s: %s\n", func, nng_strerror(rv));
}

char*
date(void)
{
    time_t now = time(&now);
    struct tm* info = localtime(&now);
    char* text = asctime(info);
    text[strlen(text) - 1] = '\0'; // remove '\n'
    return (text);
}

int
server(const char* url)
{
    nng_socket sock;
    int rv;

    if ((rv = nng_pub0_open(&sock)) != 0) {
        fatal("nng_pub0_open", rv);
    }
    if ((rv = nng_listen(sock, url, NULL, 0)) < 0) {
        fatal("nng_listen", rv);
    }
    for (;;) {
        char* d = date();
        printf("SERVER: PUBLISHING DATE %s\n", d);
        if ((rv = nng_send(sock, d, strlen(d) + 1, 0)) != 0) {
            fatal("nng_send", rv);
        }
        Sleep(2); // sleep(1);
    }
}

int
client(const char* url, const char* name)
{
    nng_socket sock;
    int rv;
    char buf[100];

    if ((rv = nng_sub0_open(&sock)) != 0) {
        fatal("nng_sub0_open", rv);
    }

    // subscribe to everything (empty means all topics)
    if ((rv = nng_setopt(sock, NNG_OPT_SUB_SUBSCRIBE, "", 0)) != 0) {
        fatal("nng_setopt", rv);
    }
    if ((rv = nng_dial(sock, url, NULL, 0)) != 0) {
        fatal("nng_dial", rv);
    }
    for (;;) {
        char* buf = NULL;
        size_t sz;
        if ((rv = nng_recv(sock, &buf, &sz, NNG_FLAG_ALLOC)) != 0) {
            fatal("nng_recv", rv);
            continue;
        }
        printf("CLIENT (%s): RECEIVED %s\n", name, buf);
        nng_free(buf, sz);
        {
            static int rcv_count = 0;
            sprintf_s(buf, 100, "received count = %d ", rcv_count++);
            if ((rv = nng_send(sock, buf, strlen(buf) + 1, 0)) != 0) {
                fatal("nng_send", rv);
            }
        }
        //Sleep(2);
    }
}

int
main(const int argc, const char** argv)
{
    if ((argc >= 2) && (strcmp(SERVER, argv[1]) == 0))
        return (server(argv[2]));

    if ((argc >= 3) && (strcmp(CLIENT, argv[1]) == 0))
        return (client(argv[2], argv[3]));

    fprintf(stderr, "Usage: pubsub %s|%s <URL> <ARG> ...\n",
        SERVER, CLIENT);
    return 1;
}