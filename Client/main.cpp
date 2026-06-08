#define _CRT_SECURE_NO_WARNINGS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // WIN32_LEAN_AND_MEAN

#include<iostream>
#include<WinSock2.h>
#include<WS2tcpip.h>
#include<iphlpapi.h>
#include<FormatLastError.h>
#include<Messages.h>
using namespace std;

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "FormatLastError.lib")

#define PORT "27015"
#define BUFFER_LENGTH	1500

CHAR recvbuffer[BUFFER_LENGTH] = {};

VOID Recieve(SOCKET connect_socket);

void main() 
{
	setlocale(LC_ALL, "");
	cout << "CLIENT" << endl;
	CHAR szError[256] = {};
	//1) Init WinSOCK:
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2, ), &wsaData);
	if (iResult != 0)
	{
		cout << "WSAStartup failed: " << iResult << endl;
		return;
	}

	//2) Задаем параметры подключения: IP-адрес сервера и порт
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
		cout << FormatLastError(WSAGetLastError(), szError) << endl;
		cout << "Socket creation error: " << WSAGetLastError()<<endl;
		freeaddrinfo(result);
		WSACleanup();
		return;
	}
	
	//4) Подключение к Серверу:
	iResult = connect(connect_socket, result->ai_addr, result->ai_addrlen);
	if (iResult == SOCKET_ERROR) 
	{
		DWORD dwError = WSAGetLastError();
		cout << "Unable to connect to Server " << endl;
		cout << FormatLastError(dwError, szError) << endl;
		closesocket(connect_socket);
		freeaddrinfo(result);
		WSACleanup();
		return;
	}

	//5) Отправка и получение данных:
	DWORD dwReciveThreadID = 0;
	HANDLE hReceiveThread = CreateThread
	(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)Recieve, 
		(LPVOID)connect_socket, 
		0,
		&dwReciveThreadID
	);
	//Создание отдельного потока на recive (получение) данных для каждого клиента
	CHAR sendbuffer[BUFFER_LENGTH] = "Hello Server";
	do
	{
		//CHAR recvbuffer[BUFFER_LENGTH] = {};
		iResult = send(connect_socket, sendbuffer, strlen(sendbuffer), 0);
		//send - функция которая позволяет отправить сообщение по связанному сокету, к примеру: один 
		//участник отправляет собщение второму участнику, или серверу
		//SOCKET - аргумент принимающий сокет через который идёт содениение
		//*buf - указатель на массив который проинициализирован уже хранит какое-то сообщение для 
		//отправки получателю используя сокет идущий первым аргументом как маршрутизатор (куръерскую
		//службу или почтовое отделение).
		//len - размер массива, его длина.
		if (iResult == SOCKET_ERROR)
		{
			cout << FormatLastError(WSAGetLastError(), szError) << endl;
			cout << "Send failed:\t" << WSAGetLastError() << endl;
			closesocket(connect_socket);
			freeaddrinfo(result);
			WSACleanup();
			return;
		}
		cout << "Bytes sent: " << iResult << endl;
		ZeroMemory(sendbuffer, BUFFER_LENGTH);
		SetConsoleCP(1251);
		//Меняем кодировку на кирилицу
		cin.getline(sendbuffer, BUFFER_LENGTH);
		SetConsoleCP(866);
		//Меняем кодировку на латиницу
	} while (strcmp(sendbuffer, "exit") && strcmp(recvbuffer, DECLINE_MESSAGE) != 0);

	iResult = shutdown(connect_socket, SD_BOTH);
	if (iResult == SOCKET_ERROR)
	{
		cout << FormatLastError(WSAGetLastError(), szError) << endl;
		cout << "Shutdown failed: " << WSAGetLastError() << endl;
	}
	closesocket(connect_socket);
	freeaddrinfo(result);
	WSACleanup();
}

VOID Recieve(SOCKET connect_socket) 
{
	DWORD dwError = 0;
	CHAR szError[256] = {};
	INT iResult = 0;
	do
	{
		ZeroMemory(recvbuffer, sizeof(recvbuffer));
		iResult = recv(connect_socket, recvbuffer, BUFFER_LENGTH, 0);
		//recv - функция которая позволяет получить сообщение по связанному сокету, к примеру: один 
		//участник получает собщение от второго участника, или сервера
		//SOCKET - аргумент принимающий сокет через который идёт содениение
		//*buf - указатель на массив в который будет загружено полученое сообщение
		//len - размер массива, его длина.
		//flags - флаг на особое повдение функции
	if (iResult > 0)
	{
		cout << recvbuffer << "(" << iResult << " Bytes)" << endl;
	}
	else cout << FormatLastError(WSAGetLastError(), szError) << endl;
	} while (strcmp(recvbuffer,DECLINE_MESSAGE) != 0);
	//Поток не будет выполнять без цикла, в потоке запускается то что в цикле
	if (strcmp(recvbuffer, DECLINE_MESSAGE) == 0)
	{
		cout << "Для выхода нажмите Enter" << endl;
		//system("PAUSE");
		//ожидает нажатие клавиши.
		//break;
	}
}