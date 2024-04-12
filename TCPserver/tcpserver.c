#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>

void quit(const char* msg) {
	printf("Error: %s\n", msg);
	exit(1);

}
int main(int argc, char** argv) { // this program takes port number from the user
	if (argc != 2) quit("Command argument");
	WSADATA w;
	int ret = WSAStartup(MAKEWORD(2, 2), &w);
	if (ret == 1) quit("Winsock API");
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == SOCKET_ERROR) quit("Create socket");
	printf("Socket is ready\n");
	SOCKADDR_IN sa, ca; // local/remote socket address 
	sa.sin_addr.s_addr = INADDR_ANY;
	sa.sin_port = htons(atoi(argv[1]));
	sa.sin_family = AF_INET;
	ret = bind(s, (SOCKADDR*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR) quit("binding");
	printf("Binding is ok\n");
	ret = listen(s, 5);
	if (ret == SOCKET_ERROR) quit("listen");
	printf("SERVER is listening on port %s\n", argv[1]);
	while (1){
		int clen = sizeof(ca);
		SOCKET cs = accept(s, (SOCKADDR*)&ca, &clen);
		if (cs == INVALID_SOCKET) quit("accept");
		printf("Client %s:%d is connected\n", inet_ntoa(ca.sin_addr), ntohs(ca.sin_port));
		//from this moment, we should make multiprocess programming
		while (1) { // communication with the client
			char buf[1000] = { 0 };
			ret = recv(cs, buf, 1000, 0);
			if (ret == SOCKET_ERROR) quit("recv");
			printf("\tClient >> %s\n", buf);
			if (buf[0] == '#')break;
			printf("RETURN :: ");
			memset(buf, 0, 1000);
			fgets(buf, 1000, stdin);
			ret = send(cs, buf, strlen(buf), 0);

		}
		closesocket(cs);
		break;
	}
}
