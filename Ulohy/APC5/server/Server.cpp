/*
	Server application, in c
	For managing tcp connection is used code from this source:
	https://msdn.microsoft.com/en-us/library/windows/desktop/ms737593(v=vs.85).aspx
	Only manages one connection at time
	at the beginning waits for connection 20 seconds

*/

#include <winsock2.h>
#include <stdio.h>

//fragment length set to 4 bytes
#define FRAGMENT_LENGTH 4096


// A sample of the select() return value

int recvTimeOutTCP(SOCKET socket, long sec, long usec)
{	
	struct timeval timeout;
	struct fd_set fds;

	// assign the second and microsecond variables
	timeout.tv_sec = sec;
	timeout.tv_usec = usec;

	// Setup fd_set structure
	FD_ZERO(&fds);
	FD_SET(socket, &fds);

	// Possible return values:

	// -1: error occurred

	// 0: timed out

	// > 0: data ready to be read

	return select(0, &fds, 0, 0, &timeout);

}


//argument: port
int main(int argc, char **argv)

{

	WSADATA            wsaData;
	SOCKET             ListeningSocket, NewConnection;
	SOCKADDR_IN        ServerAddr, SenderInfo;		
	int				SelectTiming;
	

	// Initialize Winsock version 2.2

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		printf("Server: WSAStartup failed with error %ld.\n", WSAGetLastError());
		return 1;
	}	
	
	
	ListeningSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ListeningSocket == INVALID_SOCKET)
	{
		printf("Server: Error at socket(), error code: %ld.\n", WSAGetLastError());
		WSACleanup();
		return 1;
	}
	else printf("Server: socket() is OK!\n");


	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons(atoi(argv[1]));
	ServerAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	

	if (bind(ListeningSocket, (SOCKADDR *)&ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		printf("Server: bind() failed! Error code: %ld.\n", WSAGetLastError());
		closesocket(ListeningSocket);
		WSACleanup();
		return 1;
	}
	else printf("Server: bind() is OK!\n");
	

	// Listen for client connections with a backlog of 5

	if (listen(ListeningSocket, 5) == SOCKET_ERROR)
	{
		printf("Server: listen(): Error listening on socket %ld.\n", WSAGetLastError());
		closesocket(ListeningSocket);
		WSACleanup();
		return 1;
	}
	else printf("Server: listen() is OK, I'm listening for connections...\n");
	

	// Set 20 seconds 20 useconds timeout

	SelectTiming = recvTimeOutTCP(ListeningSocket, 20, 20);
	

	switch (SelectTiming)
	{
	case 0:
		printf("\nServer: Timeout lor while waiting you retard client!...\n");
		break;

	case -1:
		printf("\nServer: Some error encountered with code number: %ld\n", WSAGetLastError());
		break;

	default:
	{
		while (1)
		{
			NewConnection = SOCKET_ERROR;
			while (NewConnection == SOCKET_ERROR)
			{
				//accept connection
				NewConnection = accept(ListeningSocket, NULL, NULL);				

				//output file
				FILE *output_file;
				output_file = fopen("file", "wb");
				if (output_file == NULL)
				{
					printf("Error opening file\n");
					return 1;
				}

				//hash
				unsigned long long* hash = (unsigned long long*)malloc(sizeof(unsigned long long));
				*hash = 0;
				
				char recv_buffer[FRAGMENT_LENGTH];
				memset(recv_buffer, '0', sizeof(recv_buffer));
				
				while (1)
				{
					int retcode = recv(NewConnection, recv_buffer, FRAGMENT_LENGTH, 0);
					if (retcode <= 0)
					{
						break;
					}

					(*hash)+=retcode;					
					fwrite(recv_buffer, 1, retcode, output_file);
				}
				printf("Received\nhash: %llu\n", *hash);

				//send hash
				int retcode = send(NewConnection, hash, sizeof(unsigned long long), 0);
				printf("Hash sent\n");

				free(hash);

			}

			shutdown(NewConnection, SD_SEND);			

			if (recvTimeOutTCP(ListeningSocket, 15, 0) == 0)
			{
				printf("\nConnection time out\n");
				break;
			}
				

		}

	}

	}
	
	closesocket(ListeningSocket);
	WSACleanup();



	return 0;

}