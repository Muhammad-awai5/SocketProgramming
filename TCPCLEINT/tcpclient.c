#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>

void quit(const char* msg) {
	printf("Error: %s\n", msg);
	exit(1);

}
int main(int argc, char** argv) { // this program takes port number from the user
	if (argc != 3) quit("Command argument"); // client also needs server IP
	WSADATA w;
	int ret = WSAStartup(MAKEWORD(2, 2), &w);
	if (ret == 1) quit("Winsock API");
	SOCKET s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (s == SOCKET_ERROR) quit("Create socket");
	printf("Socket is ready\n");
	SOCKADDR_IN sa; // sa is remote socket address 
	sa.sin_addr.s_addr = inet_addr(argv[1]); // server ip
	sa.sin_port = htons(atoi(argv[2])); // server port
	sa.sin_family = AF_INET;

	printf("TCP client is ok\n");
	ret = connect(s, (SOCKADDR*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR) quit("connect");
	printf("GOT The SERVER!\n");
	while (1) {
		char buf[1000] = { 0 };// "GET /~gc/index.html HTTP/1.1\r\nHOST: www.cc.puv.fi\r\n\r\n";//{ 0 };
		printf("Client to send (starting '#' with terminate):");
		fgets(buf, 1000, stdin);
		ret = send(s, buf, strlen(buf), 0);
		if (ret == SOCKET_ERROR) quit("send");
		if (buf[0] == '#')break;
		memset(buf, 0, 1000);
		ret = recv(s, buf, 1000, 0);
		if (ret == SOCKET_ERROR) quit("recv");
		printf("\tServer::%s\n", buf);
		
	
	}
	closesocket(s);
}
