#pragma comment(lib, "ws2_32.lib")
#include <stdio.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>

void quit(const char* msg) {
	printf("Error: %s\n", msg);
	exit(1);

}
int main(int argc, char ** argv) { // this program takes port number from the user
	if (argc != 2) quit("Command argument");
	WSADATA w;
	int ret = WSAStartup(MAKEWORD(2, 2), &w);
	if (ret == 1) quit("Winsock API");
	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == SOCKET_ERROR) quit("Create socket");
	printf("Socket is ready\n");
	SOCKADDR_IN sa,ca; // local/remote socket address 
	sa.sin_addr.s_addr = INADDR_ANY;
	sa.sin_port = htons(atoi(argv[1]));
	sa.sin_family = AF_INET;
	ret = bind(s, (SOCKADDR*)&sa, sizeof(sa));
	if (ret == SOCKET_ERROR) quit("binding");
	printf("Binding is ok\n");
	while (1){
		char buf[1000] = { 0 };//memset(buf, 0 , 100);
		int rlen = sizeof(ca);
		ret = recvfrom(s, buf, 1000, 0, (SOCKADDR*)&ca , &rlen); //in case if we dont wanna know the remote
		if (ret == SOCKET_ERROR) quit("recvform");
		printf("Client:(%s:%d): %s\n", inet_ntoa(ca.sin_addr), ntohs(ca.sin_port), buf);
		printf("\tServer Reply: ");
		memset(buf, 0, 1000);   //clear buffer for sending
		fgets(buf, 1000, stdin); // take a line of message 
		ret = sendto(s, buf, strlen(buf), 0, (SOCKADDR*)&ca, sizeof(ca));


	}

}