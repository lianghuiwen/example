#include <winsock2.h>
#include <iostream>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main(int argc, char* argv[])
{
    WSADATA wsaData; 
    if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0) {
        cout << "init windows socket failed: " << GetLastError() << endl;
        return -1;
    }

    SOCKET listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(listenfd == INVALID_SOCKET) {
        cout << "create socket failed: " << GetLastError() << endl;
        return -1;
    }

    int port = atoi(argv[1]);
    sockaddr_in addr;
    ZeroMemory(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(port);

    if(bind(listenfd, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
        cout << "bind socket failed: " << GetLastError() << endl;
        return -1;
    }

    int backlog = 5;
    if(listen(listenfd, backlog) != 0) {
        cout << "listen socket failed: " << GetLastError() << endl;
        return -1;
    }

    cout << "start listening " << port << endl;
    for (;;) {
        struct sockaddr_in peeraddr;
        ZeroMemory(&peeraddr, sizeof(peeraddr));
        int addrlen = sizeof(peeraddr);

        int sockfd = accept(listenfd, (struct sockaddr*) &peeraddr, &addrlen);
        if (sockfd > 0) {
            cout << inet_ntoa(peeraddr.sin_addr) << " connected." << endl;
            for (;;) {
                char buf[255];
                ZeroMemory(&buf, sizeof(buf));
                int ret = recv(sockfd, buf, sizeof(buf), 0);
                if (ret > 0) {
                    cout << inet_ntoa(peeraddr.sin_addr) << " >> " << buf << endl;
                } else {
                    cout << inet_ntoa(peeraddr.sin_addr) << " disconnected!" << endl;
                    break;
                }
            }
            closesocket(sockfd);
        }
    }

    closesocket(listenfd);
    WSACleanup();
    return 0;
}
