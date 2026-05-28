#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif //  WIN32_LEAN_AND_MEAN


#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
using namespace std;

#pragma comment(lib, "WS2_32.lib")

#define PORT "27015"
#define BUFFER_LENGTH	1500

void main() 
{
	setlocale(LC_ALL, "");
	cout << "CLIENT" << endl;
	//1) Init WinSOCK:
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2, ), &wsaData);
	if (iResult != 0)
	{
		cout << "WSAStartup failed: " << iResult << endl;
		return;
	}

	//2) Задаем параметры :
	struct addrinfo hinsts;
	struct addrinfo* result;
	ZeroMemory(&hinsts, sizeof(hinsts));
	hinsts.ai_family = AF_INET;
	hinsts.ai_socktype = SOCK_STREAM;
	hinsts.ai_protocol = IPPROTO_TCP;
	iResult = getaddrinfo("127.0.0.1", PORT, &hinsts, &result);
	if (iResult != 0) 
	{
		cout << "getaddressinfo() failed: " << iResult << endl;
		WSACleanup();
		return;
	}

	//3) Создем клиентский сокет:
	SOCKET connect_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (connect_socket == INVALID_SOCKET) 
	{
		cout << "Socket creation error: " << WSAGetLastError()<<endl;
		freeaddrinfo(result);
		WSACleanup();
		return;
	}
	
	//4) Подключение к Сереверу:
	iResult = connect(connect_socket, result->ai_addr, result->ai_addrlen);
	if (iResult == SOCKET_ERROR) 
	{
		cout << "Unable to connect to Server" << endl;
		closesocket(connect_socket);
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	//5)Отправка и получение данных:
	CHAR sendbuffer[BUFFER_LENGTH] = "Hello Server";
	CHAR recvbuffer[BUFFER_LENGTH] = {};

	iResult = send(connect_socket, sendbuffer, strlen(sendbuffer), 0);
	if (iResult == SOCKET_ERROR) 
	{
		cout << "Send failed:\t" << WSAGetLastError << endl;
		closesocket(connect_socket);
		freeaddrinfo(result);
		WSACleanup();
		return;
	}
	cout << "Bytes sent: " << iResult << endl;
	do 
	{
		iResult = recv(connect_socket, recvbuffer, BUFFER_LENGTH, 0);
		if (iResult > 0)cout << recvbuffer << "(" << iResult << " Bytes)" << endl;
		else if (result == 0) cout << "Connection clossed" << endl;
		else cout << "Recieve failed:\t"<<WSAGetLastError() << endl;
	} while (iResult > 0);

	iResult = shutdown(connect_socket, SD_BOTH);
	if (iResult == SOCKET_ERROR)
	{
		cout << "Shutdown failed: " << WSAGetLastError() << endl;
		closesocket(connect_socket);
		freeaddrinfo(result);
		WSACleanup();
		return;
	}
}