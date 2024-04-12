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
	SOCKET s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (s == SOCKET_ERROR) quit("Create socket");
	printf("Socket is ready\n");
	SOCKADDR_IN sa; // sa is remote socket address 
	sa.sin_addr.s_addr = inet_addr(argv[1]); // server ip
	sa.sin_port = htons(atoi(argv[2])); // server port
	sa.sin_family = AF_INET;
	
	printf("UDP client is ok\n");
	while (1) {
		char buf[1000] = { 0 };//memset(buf, 0 , 100);
		printf("Client:: ");
		fgets(buf, 1000, stdin); // take a line of message 
		ret = sendto(s, buf, strlen(buf), 0, (SOCKADDR*)&sa,sizeof(sa));
		if (ret == SOCKET_ERROR) quit("sendto"); //error handling 
		memset(buf, 0, 1000);   //clear buffer for sending
		ret = recvfrom(s, buf, 1000, 0, NULL, NULL);
		if (ret == SOCKET_ERROR) quit("recfrom");
		printf("\tServer>> %s\n", buf);

		


	}
	
	
}