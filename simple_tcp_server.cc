#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        printf("create socket error: %s(errno: %d)\n", strerror(errno), errno); 
        return -1;
    }

    int port = atoi(argv[1]);
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);
    if (bind(listenfd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
        printf("bind socket error: %s(errno: %d)\n", strerror(errno), errno); 
        return -1;
    }

    int backlog = 5;
    if (listen(listenfd, backlog) == -1) {
        printf("listen socket error: %s(errno: %d)\n", strerror(errno), errno);
        return -1;
    }
    
    printf("start listening %d\n", port);
    for (;;) {
        struct sockaddr_in peeraddr;
        bzero(&peeraddr, sizeof(peeraddr));
        size_t addrlen = sizeof(peeraddr);

        int sockfd = accept(listenfd, (struct sockaddr*) &peeraddr, (socklen_t*) &addrlen);
        if (sockfd > 0) {
            char ip[32];
            inet_ntop(AF_INET, &peeraddr.sin_addr, ip, sizeof(ip));
            printf("%s connected.\n", ip);

            for (;;) {
                char buf[255];
                bzero(&buf, sizeof(buf));
                int ret = read(sockfd, buf, sizeof(buf)-1);
                if (ret > 0) {
                    printf("%s >> %s\n", ip, buf);
                } else {
                    printf("%s disconnected!\n", ip);
                    break;
                }
            }
            close(sockfd);
        }
    }

    close(listenfd);
    return 0;
}
