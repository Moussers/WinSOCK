#define _CRT_SECURE_NO_WARNINGS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN

#include<iostream>
#include<Windows.h>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iphlpapi.h>
#include<FormatLastError.h>
#include <fstream>
#include<list>
#include<thread>
#include<chrono>
#include<ctime>
#include <iomanip>
using namespace std;

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "FormatLastError.lib")

#define PORT	"27015"
#define BUFFER_LENGTH 1500
#define MAX_CONNECTIONS 5

void checkClient(INT ind, SOCKET& client, ofstream& ofs, bool&running)
{
	int iResult;
	CHAR recvbuffer[BUFFER_LENGTH] = {};
	CHAR sendbuffer[BUFFER_LENGTH] = {};
	INT iSendResult = 0;
	DWORD dwError = 0;
	CHAR szError[256] = {};
	do
	{
		iResult = recv(client, recvbuffer, BUFFER_LENGTH, 0);
		dwError = WSAGetLastError();
		if (iResult > 0)
		{
			ofs << "USER " << ind <<endl;
			ofs << recvbuffer << "(" << strlen(recvbuffer) << " Bytes)" << endl;
			//ofs << - команда на запись в файл во время действия потока (ofstream)
			cout << "USER " << ind << endl; 
			cout << recvbuffer << "(" << strlen(recvbuffer) << " Bytes)" << endl;
			iSendResult = send(client, recvbuffer, strlen(recvbuffer), 0);
			dwError = WSAGetLastError();
			if (iSendResult == SOCKET_ERROR)
			{
				cout << FormatLastError(dwError, szError) << endl;
				cout << "Send failed with error" << WSAGetLastError() << endl;
				closesocket(client);
			}
			else cout << "Bytes sent: " << iSendResult << endl;
		}
		else if (iResult == 0) {
			cout << "Connection closing.." << endl;
		}
		else
		{
			cout << FormatLastError(dwError, szError) << endl;
			cout << "Receive failed with error: " << WSAGetLastError() << endl;
			closesocket(client);
		}
	} while (iResult > 0);
	running = false;
}

void main() 
{
	setlocale(LC_ALL, "");
	cout << "SERVER" << endl;
	//0.0.0.0 - все сетевые IP-адреса
	ofstream ofs;
	//ofstream - поток записи в файла
	ofs.open("userData.txt", ios::app);
	//open - открываем поток и пишем всю информацию в файла, в отличие от 
	//cout\wcout который выводит поток данных в консоль
	if (!ofs) {
		cout << "File not opened!" << endl;
		return;
	}
	DWORD dwError = 0;
	CHAR szError[256] = {};
	//1) Init WinSOCK
	WSADATA wsaData;
	int iResult;
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	dwError = WSAGetLastError();
	if (iResult != 0) 
	{
		cout << FormatLastError(dwError, szError) << endl;
		cout << "WSASturtup failed: " << iResult << endl;
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
		cout << FormatLastError(dwError, szError) << endl;
		cout << "getaddrinfo() failed: " << iResult << endl;
		WSACleanup();
		return;
	}

	//3) Создаем сокет для сервера, который он будет постоянно слушать "LISTENING":
	SOCKET listen_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	dwError = WSAGetLastError();
	if (listen_socket == INVALID_SOCKET) 
	{
		cout << FormatLastError(dwError, szError) << endl;
		cout << "Listen socket error: " << WSAGetLastError() << endl;
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	//4) Bind socket:
	iResult = bind(listen_socket, result->ai_addr, result->ai_addrlen);
	dwError = GetLastError();
	if (iResult == SOCKET_ERROR) 
	{
		cout << FormatLastError(dwError, szError) << endl;
		cout << "Bind failed with error: " << WSAGetLastError() << endl;
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
		cout << FormatLastError(dwError, szError) << endl;
		cout << "Listen failed with error: " << WSAGetLastError() << endl;
		closesocket(listen_socket);
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	//6) Обработка соединений от клиентов:
	SOCKET client_socket = accept(listen_socket, NULL, NULL);
	list<bool> clientLists;
	list<thread>threads;
	clientLists.push_back(true);
	threads.push_back(thread(checkClient,1, ref(client_socket), ref(ofs), ref(clientLists.back())));
	dwError = WSAGetLastError();
	if (client_socket == INVALID_SOCKET) 
	{
		cout << FormatLastError(dwError, szError) << endl;
		cout << "Accept failed with error: " << WSAGetLastError() << endl;
	}
	sockaddr_in addr;
	//sockaddr_in - структура дя хранения адреса
	int namelen = sizeof(sockaddr_in);
	if (!getpeername(client_socket, (sockaddr*)&addr, &namelen)) 
	//getpeername - позволяет получить данные из сокета (SOCKET) и записать их в указатель sockaddr
	{
		char clientIP[1024];
		inet_ntop(AF_INET, &addr.sin_addr, clientIP, 1024);
		//sin_addr - метод стрктуры sockaddr_in позволяющий получить адрес клиента.
		//inet_ntop - принимает адрес IPv4/IPv6 и преобразует в строку и записывает в строковую переменную идущую третьим аргументом.
		//PSTR (Pointer String) - указатель на строку или по другому char-массив
		int clientPort = ntohs(addr.sin_port);
		//sin_port - метод стрктуры sockaddr_in позволяющий получить порт клиента.
		//ntohs - преобразует в номралльное целое число (int).
		cout << "IP Address: " << clientIP << endl;
		ofs << "IP Address: " << clientIP;
		cout << "PORT: " << clientPort << endl;
		ofs << "PORT: "<< clientPort << endl;
		auto now = std::chrono::system_clock::now();
		std::time_t time_now = std::chrono::system_clock::to_time_t(now);
		std::tm* local_time = std::localtime(&time_now);
		cout << "Time connection: " << std::put_time(local_time, "%d-%m-%Y %H:%M:%S") << endl;
		ofs << "Time connection: " << std::put_time(local_time, "%d-%m-%Y %H:%M:%S") << endl;
	}
	else 
	{
		cout << FormatLastError(WSAGetLastError(), szError)<<endl;
	}
	//7) Получение и отправка данных:
	CHAR recvbuffer[BUFFER_LENGTH] = {};
	CHAR sendbuffer[BUFFER_LENGTH] = {};
	INT iSendResult = 0;
	bool k;
	do 
	{
		k = false;
		timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		fd_set readSet;
		FD_ZERO(&readSet);
		FD_SET(listen_socket, &readSet);
		int act = select(0, &readSet, NULL, NULL, &timeout);
		if (act > 0) {
			client_socket = accept(listen_socket, NULL, NULL);
			clientLists.push_back(true);
			threads.push_back(thread(checkClient, 1, ref(client_socket), ref(ofs), ref(clientLists.back())));
		}
		for (const bool &client : clientLists)
		{
			k = k || client;
		}
	} while (k);
	cout << "END for Server" << endl;
	for (thread& item : threads) {
		item.join();
	}
	iResult = shutdown(client_socket, SD_BOTH);
	dwError = WSAGetLastError();
	if (iResult == SOCKET_ERROR) cout << "Client shutdown failed with " << FormatLastError(dwError, szError) << endl;

	iResult = shutdown(listen_socket, SD_BOTH);
	dwError = WSAGetLastError();
	if (iResult == SOCKET_ERROR) cout << "Server shutdown failed with " << FormatLastError(dwError, szError) << endl;
	closesocket(client_socket);
	closesocket(listen_socket);
	WSACleanup();
	ofs.close();
	//ofs.close() - закрываем поток и сам файл.
}