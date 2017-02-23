/*
	client application, in c
	- after sending file program is waiting for hash. When received, displays it and ends
	hash: number of bytes received by server

	author: Ondrej Selecký
*/
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>



//fragment length set to 4 bytes
#define FRAGMENT_LENGTH 4096

//arguments: port, ip address, file name

int main(int argc, char **argv)
{
	WSADATA              wsaData;
	SOCKET               SendingSocket;
	SOCKADDR_IN          ServerAddr, ThisSenderInfo;
			
	int  RetCode;
	
	WSAStartup(MAKEWORD(2, 2), &wsaData);

	SendingSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SendingSocket == INVALID_SOCKET)
	{
		WSACleanup();
		return -1;
	}


	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(atoi(argv[1]));
	ServerAddr.sin_addr.s_addr = inet_addr(argv[2]);	
	
	RetCode = connect(SendingSocket, (SOCKADDR *)&ServerAddr, sizeof(ServerAddr));

	if (RetCode != 0)
	{
		printf("Client: connect() failed! Error code: %ld\n", WSAGetLastError());
		closesocket(SendingSocket);
		WSACleanup();
		return -1;
	}	

	//input file
	FILE *input_file = fopen(argv[3], "rb");
	if (input_file == NULL)
	{
		printf("File opern error");
		return 1;
	}

	fseek(input_file, 0L, SEEK_END);
	unsigned long long input_file_size = ftell(input_file);
	rewind(input_file);

	// Read data from file and send it 
	while (1)
	{

		unsigned char read_buffer[FRAGMENT_LENGTH] = { 0 };
		int bytes_read = fread(read_buffer, 1, FRAGMENT_LENGTH, input_file);		

		int retcode;
		if (bytes_read > 0)
		{			
			retcode = send(SendingSocket, read_buffer, bytes_read, 0);
		}
		else
		{
			//if file is sent, send last empty packet
			retcode = send(SendingSocket, NULL, 0, 0);
		}

		if (retcode == SOCKET_ERROR)
		{
			printf("Client: send() error %ld.\n", WSAGetLastError());
			return -1;
		}
		if (bytes_read <= 0) break;

	}
	printf("Sent\n");
	unsigned long long* hash = (unsigned long long*)malloc(sizeof(unsigned long long));
	
	//connection shutdown
	shutdown(SendingSocket, SD_SEND);

	//wait for hash
	int retcode = recv(SendingSocket, hash, sizeof(unsigned long long), 0);
	printf("file size: %llu\nhash: %llu\n", input_file_size, *hash);
	if (*hash == input_file_size) printf("Hash matches, file is correctly sent\n");
	
	closesocket(SendingSocket);
	WSACleanup();

	return 0;
}