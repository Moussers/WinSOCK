#define _WINSOCK_DEPRECATED_NO_WARNINGS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN

#include<iostream>
#include<Windows.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iphlpapi.h>
#include<FormatLastError.h>
#include<Messages.h>
using namespace std;

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "FormatLastError.lib")

#define PORT	"27015"
#define BUFFER_LENGTH 1500
#define MAX_CONNECTIONS 3

SOCKET sockets[MAX_CONNECTIONS] = {};
//массив сокетов
DWORD dwThreadIDs[MAX_CONNECTIONS] = {};
//массив идентификаторов (номеров) потоков
HANDLE hThreads[MAX_CONNECTIONS] = {};
//массив дескрипторов потоков
INT g_activeClients = 0;	//счетчик клиентов

VOID ClientHandle(SOCKET client_socket);
INT GetSlotIndex(DWORD dwID);
VOID Shift(INT start);
//VOID Release(SOCKET client_socket);
VOID ShowActiveClients();
VOID Broadcast(CHAR sz_message[], DWORD dwID);

void main() 
{
	setlocale(LC_ALL, "");
	std::cout << "SERVER" << endl;
	//0.0.0.0 - все сетевые IP-адреса
	DWORD dwError = 0;
	CHAR szError[256] = {};
	//1) Init WinSOCK
	WSADATA wsaData;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	dwError = WSAGetLastError();
	if (iResult != 0) 
	{
		std::cout << FormatLastError(dwError, szError) << endl;
		std::cout << "WSASturtup failed: " << iResult << endl;
		return;
	}

	//2) Параметры подключения:
	addrinfo hints;
	addrinfo* result;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	dwError = WSAGetLastError();
	iResult = getaddrinfo(NULL, PORT, &hints, &result);
	if (iResult != 0) 
	{
		std::cout << FormatLastError(dwError, szError) << endl;
		std::cout << "getaddrinfo() failed: " << iResult << endl;
		WSACleanup();
		return;
	}

	//3) Создаем сокет для сервера, который он будет постоянно слушать "LISTENING":
	SOCKET listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	dwError = WSAGetLastError();
	if (listen_socket == INVALID_SOCKET) 
	{
		std::cout << FormatLastError(dwError, szError) << endl;
		std::cout << "Listen socket error: " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	//4) Bind socket:
	iResult = bind(listen_socket, result->ai_addr, result->ai_addrlen);
	dwError = GetLastError();
	if (iResult == SOCKET_ERROR) 
	{
		std::cout << FormatLastError(dwError, szError) << endl;
		std::cout << "Bind failed with error: " << WSAGetLastError() << endl;
		closesocket(listen_socket);
		freeaddrinfo(result);
		WSACleanup();
		return;
	}
	freeaddrinfo(result);

	//5) Запускаем прослушивание сокета:
	if (listen(listen_socket, MAX_CONNECTIONS) == SOCKET_ERROR) 
	{
		dwError = WSAGetLastError();
		std::cout << FormatLastError(dwError, szError) << endl;
		std::cout << "Listen failed with error: " << WSAGetLastError() << endl;
		closesocket(listen_socket);
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	//6) Обработка соединений от клиентов:
	
	do
	{
		ShowActiveClients();
		sockaddr_in client_address;
		int client_addrlen = sizeof(client_address);
		client_address.sin_family = AF_INET;
		SOCKET client_socket = accept(listen_socket, (SOCKADDR*)&client_address, &client_addrlen);
		dwError = WSAGetLastError();
		if (client_socket == INVALID_SOCKET)
		{
			std::cout << FormatLastError(dwError, szError) << endl;
			std::cout << "Accept failed with error: " << WSAGetLastError() << ":" << endl;
		}
		
		//6.1) Получаем информацию о сокете клиента:
		std::cout << inet_ntoa(client_address.sin_addr) << ":" << ntohs(client_address.sin_port) << endl;
		if (g_activeClients < MAX_CONNECTIONS)
		{
			sockets[g_activeClients] = client_socket;
			std::cout << client_socket << "\t" << endl;
			hThreads[g_activeClients] = CreateThread(
				NULL,	//Security attrebutes
				0,		//Stack Size
				(LPTHREAD_START_ROUTINE)ClientHandle,	//Указатель на функцию которая будет выполняться в потоке
				(LPVOID) sockets[g_activeClients],
				0,
				&dwThreadIDs[g_activeClients]		//указатель на массив идентификаторов (номеров) потоков, который будет 
													//содержать эти потоки
			);
			g_activeClients++;
		}
		else 
		{
			CHAR recv_buffer[BUFFER_LENGTH] = {};
			iResult = recv(client_socket, recv_buffer, BUFFER_LENGTH, NULL);
			if (iResult != 0)
			{
				FormatLastError(WSAGetLastError(), szError);
				std::cout << szError << endl;
			}
			else std::cout << recv_buffer << endl;
			iResult = send(client_socket, DECLINE_MESSAGE, strlen(DECLINE_MESSAGE), NULL);
			shutdown(client_socket, SD_BOTH);
			closesocket(client_socket);

		}
	} while (true);
	WaitForMultipleObjects(MAX_CONNECTIONS, hThreads, TRUE, INFINITE);

	/*iResult = shutdown(listen_socket, SD_RECEIVE);
	dwError = WSAGetLastError();
	if (iResult == SOCKET_ERROR) cout << "Server shutdown failed with " << FormatLastError(dwError, szError) << endl;*/
	
	closesocket(listen_socket);
	WSACleanup();
}
INT GetSlotIndex (DWORD dwID)
{
	for (INT i = 0; i < MAX_CONNECTIONS; i++) 
	{
		if (dwThreadIDs[i] = dwID) return i;
	}
}
VOID Shift(INT start) 
{
	for (INT i = 0; i < MAX_CONNECTIONS; i++) 
	{
		sockets[i] = sockets[i + 1];
		dwThreadIDs[i] = dwThreadIDs[i + 1];
		hThreads[i] = hThreads[i + 1];
	}
	sockets[MAX_CONNECTIONS - 1] = NULL;
	dwThreadIDs[MAX_CONNECTIONS - 1] = NULL;
	hThreads[MAX_CONNECTIONS - 1] = NULL;
	g_activeClients--;
}
VOID ClientHandle(SOCKET client_socket) 
{
	sockaddr_in client_address;
	client_address.sin_family = AF_INET;
	INT namelen = sizeof(client_address);
	getpeername(client_socket, (sockaddr*) &client_address, &namelen);
	//getpeername - позволяет получить данные клиента из сокета (SOCKET) и записать их в указатель sockaddr
	//третий параметр отвечает за размер буфера имени узла
	CHAR sz_client_address[32] = {};
	CHAR sz_client_connected[32] = {};
	sprintf(sz_client_address, "%s:%d - ", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
	sprintf(sz_client_connected, "%s:%d - ", inet_ntoa(client_address.sin_addr), ntohs(client_address.sin_port));
	//Broadcast(sz_client_connected, GetCurrentThreadId());
	std::cout << "Client connected:\t" << sz_client_address << "\tSOCKET:\t" <<client_socket<< endl;
	INT iResult = 0;
	DWORD dwError;
	//7) Получение и отправка данных:
	INT iSendResult = 0;
	do
	{
		CHAR sendbuffer[BUFFER_LENGTH] = {};
		CHAR recvbuffer[BUFFER_LENGTH] = {};
		iResult = recv(client_socket, recvbuffer, BUFFER_LENGTH, 0);
		dwError = WSAGetLastError();
		if (iResult > 0)
		{
			std::cout << sz_client_address << recvbuffer << "(" << strlen(recvbuffer) << " Bytes)" << endl;
			Broadcast(recvbuffer, GetCurrentThreadId());
			iSendResult = send(client_socket, recvbuffer, strlen(recvbuffer), 0);
			dwError = WSAGetLastError();
			if (iSendResult == SOCKET_ERROR)
			{
				std::cout << FormatLastError(dwError, sz_client_address) << endl;
				std::cout << "Send failed with error" << WSAGetLastError() << endl;
				closesocket(client_socket);
			}
			else std::cout << "Bytes sent: " << iSendResult << endl;
		}
		else if (iResult == 0) std::cout << "Connection closing.." << endl;
		else
		{
			std::cout << FormatLastError(dwError, sz_client_address) << endl;
			std::cout << "Receive failed with error: " << WSAGetLastError() << endl;
			closesocket(client_socket);
		}
	} while (iResult > 0);
	DWORD dwID = GetCurrentThreadId();
	Shift(GetSlotIndex(dwID));
	std::cout << sz_client_address << "left" << endl;
	iResult = shutdown(client_socket, SD_BOTH);
	dwError = WSAGetLastError();
	if (iResult == SOCKET_ERROR) std::cout << "Client shutdown failed with " << FormatLastError(dwError, sz_client_address) << endl;
	closesocket(client_socket);
	//Release(client_socket);
	ShowActiveClients();
	ExitThread(0);
}

/*VOID Release(SOCKET client_socket)
{
	for (int i = 0; i < MAX_CONNECTIONS; i++)
	{
		if (client_socket == sockets[i])
		{
			sockets[i] = NULL;
			//dwThreadIDs[i] = NULL;
			//hThreads[i] == NULL;
			for (int j = i; sockets[j] || j < MAX_CONNECTIONS-1; j++) 
			{
				sockets[j] = sockets[j + 1];
				dwThreadIDs[j] = dwThreadIDs[j + 1];
				hThreads[j] = hThreads[j + 1];
			}
		}
	}
	g_activeClients--;
	ShowActiveClients();
}
*/

VOID ShowActiveClients() 
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(hConsole, &info);
	COORD cursor = { 1, 25 };
	SetConsoleCursorPosition(hConsole, cursor);
	std::cout << "Количество подключений: " << g_activeClients;
	SetConsoleCursorPosition(hConsole, info.dwCursorPosition);
}
VOID Broadcast(CHAR sz_message[], DWORD dwID) 
{
	for (int i = 0; i < g_activeClients; ++i) 
	{
		if (dwThreadIDs[i] != dwID) 
			send(sockets[i], sz_message, strlen(sz_message), 0);
	}
}