//#define WIN32_LEAN_AND_MEAN
//
//#include <windows.h>
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#include <stdlib.h>
//#include <stdio.h>
//
//
//// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
//#pragma comment (lib, "Ws2_32.lib")
//#pragma comment (lib, "Mswsock.lib")
//#pragma comment (lib, "AdvApi32.lib")
//
//
//#define DEFAULT_BUFLEN 512
//#define DEFAULT_PORT "27015"
//
//int __cdecl main(int argc, char** argv)
//{
//    WSADATA wsaData;
//    SOCKET ConnectSocket = INVALID_SOCKET;
//    struct addrinfo* result = NULL,
//        * ptr = NULL,
//        hints;
//    const char* sendbuf = "this is a test";
//    char recvbuf[DEFAULT_BUFLEN];
//    int iResult;
//    int recvbuflen = DEFAULT_BUFLEN;
//
//    // Validate the parameters
//    if (argc != 2) {
//        printf("usage: %s server-name\n", argv[0]);
//        return 1;
//    }
//
//    // Initialize Winsock
//    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
//    if (iResult != 0) {
//        printf("WSAStartup failed with error: %d\n", iResult);
//        return 1;
//    }
//
//    ZeroMemory(&hints, sizeof(hints));
//    hints.ai_family = AF_UNSPEC;
//    hints.ai_socktype = SOCK_STREAM;
//    hints.ai_protocol = IPPROTO_TCP;
//
//    // Resolve the server address and port
//    iResult = getaddrinfo(argv[1], DEFAULT_PORT, &hints, &result);
//    if (iResult != 0) {
//        printf("getaddrinfo failed with error: %d\n", iResult);
//        WSACleanup();
//        return 1;
//    }
//
//    // Attempt to connect to an address until one succeeds
//    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {
//
//        // Create a SOCKET for connecting to server
//        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
//            ptr->ai_protocol);
//        if (ConnectSocket == INVALID_SOCKET) {
//            printf("socket failed with error: %ld\n", WSAGetLastError());
//            WSACleanup();
//            return 1;
//        }
//
//        // Connect to server.
//        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
//        if (iResult == SOCKET_ERROR) {
//            closesocket(ConnectSocket);
//            ConnectSocket = INVALID_SOCKET;
//            continue;
//        }
//        break;
//    }
//
//    freeaddrinfo(result);
//
//    if (ConnectSocket == INVALID_SOCKET) {
//        printf("Unable to connect to server!\n");
//        WSACleanup();
//        return 1;
//    }
//
//    // Send an initial buffer
//    iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
//    if (iResult == SOCKET_ERROR) {
//        printf("send failed with error: %d\n", WSAGetLastError());
//        closesocket(ConnectSocket);
//        WSACleanup();
//        return 1;
//    }
//
//    printf("Bytes Sent: %ld\n", iResult);
//
//    // shutdown the connection since no more data will be sent
//    iResult = shutdown(ConnectSocket, SD_SEND);
//    if (iResult == SOCKET_ERROR) {
//        printf("shutdown failed with error: %d\n", WSAGetLastError());
//        closesocket(ConnectSocket);
//        WSACleanup();
//        return 1;
//    }
//
//    // Receive until the peer closes the connection
//    do {
//
//        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
//        if (iResult > 0)
//            printf("Bytes received: %d\n", iResult);
//        else if (iResult == 0)
//            printf("Connection closed\n");
//        else
//            printf("recv failed with error: %d\n", WSAGetLastError());
//
//    } while (iResult > 0);
//
//    // cleanup
//    closesocket(ConnectSocket);
//    WSACleanup();
//
//    return 0;
//}




#include<iostream>
#include<WS2tcpip.h>
#include<string>

#pragma comment(lib, "ws2_32.lib")

using namespace std;
#define PORT 8080
int main() {
	string ipAddress = "127.0.0.1"; //ipaddress of server

	/*cout << " input IP Address of Server:";
	getline(std::cin, ipAddress);*/

	//initialise winsock

	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);

	if (wsResult != 0) {
		cout << "can't start winsock, error #" << wsResult << endl;
		return 1;
	}
	else cout << "winsock started" << endl;

	//create winsock

	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cout << "can't create socket #" << WSAGetLastError() << endl;
		return 1;
	}
	else cout << "winsock created" << endl;
	//fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(PORT);
	//inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);
	DWORD ip = inet_addr(ipAddress.c_str());
	hint.sin_addr.S_un.S_addr = ip;
	//connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR)
	{
		cout << "can't connect to server, error#  " << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return 1;
	}
	else {
		cout << " * enter something to start * " << endl;
	}
	//do while loop 

	char buf[1024];
	string userInput;
	do {
		//Promt the user for some text

		cout << ">";
		getline(cin, userInput);

		if (userInput.size() > 0) {
			//send the text
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);

			if (sendResult != SOCKET_ERROR) {
				//Wait for respone

				int byteReceived = recv(sock, buf, 1024, 0);
				if (byteReceived == SOCKET_ERROR) {
					cout << "server disconnected " << endl;
					closesocket(sock);
					WSACleanup();
					system("PAUSE");
					return 1;
				}
				if (byteReceived <= 0) {
					cout << "server disconnected " << endl;
					closesocket(sock);
					WSACleanup();
					system("PAUSE");
					return 1;
				}
				if (byteReceived > 0) {
					//Echo respone to console

					cout << "SERVERR>" << string(buf, 0, byteReceived) << endl;

				}

			}
		}
	} while (userInput.size() > 0);
	//gracefully close down everything
	closesocket(sock);
	WSACleanup();

	return 0;
}