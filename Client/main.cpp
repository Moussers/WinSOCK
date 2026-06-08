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

#pragma comment(lib, "WS2_32.lib")
#pragma comment(lib, "FormatLastError.lib")

#define PORT "27015"
#define BUFFER_LENGTH	1500

void getMessages(SOCKET* connect_socket)
{
	do
	{
		CHAR recvbuffer[BUFFER_LENGTH] = {};
		CHAR szError[256] = {};
		int iResult = recv(*connect_socket, recvbuffer, BUFFER_LENGTH, 0);
		//recv - функция которая позволяет получить сообщение по связанному сокету, к примеру: один 
		//участник получает собщение от второго участника, или сервера
		//SOCKET - аргумент принимающий сокет через который идёт содениение
		//*buf - указатель на массив в который будет загружено полученое сообщение
		//len - размер массива, его длина.
		//flags - флаг на особое повдение функции
		if (iResult > 0)
		{
			std::cout << recvbuffer << "(" << iResult << " Bytes)" << std::endl;
		}
		else if (iResult == 0) std::cout << "Connection clossed" << std::endl;
		else std::cout << FormatLastError(WSAGetLastError(), szError) << std::endl;
		if (strcmp(recvbuffer, DECLINE_MESSAGE) == 0)
		{
			system("PAUSE");
			//ожидает нажатие клавиши.
			break;
		}
	} while (*connect_socket != INVALID_SOCKET);
}


void main() 
{
	setlocale(LC_ALL, "");
	std::cout << "CLIENT" << std::endl;
	CHAR szError[256] = {};
	//1) Init WinSOCK:
	WSADATA wsaData;
	int iResult = WSAStartup(MAKEWORD(2, 2, ), &wsaData);
	if (iResult != 0)
	{
		std::cout << "WSAStartup failed: " << iResult << std::endl;
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
		std::cout << "getaddressinfo() failed: " << iResult << std::endl;
		WSACleanup();
		return;
	}

	//3) Создем клиентский сокет:
	SOCKET connect_socket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (connect_socket == INVALID_SOCKET) 
	{
		std::cout << FormatLastError(WSAGetLastError(), szError) << std::endl;
		std::cout << "Socket creation error: " << WSAGetLastError()<<std::endl;
		freeaddrinfo(result);
		WSACleanup();
		return;
	}
	
	//4) Подключение к Серверу:
	iResult = connect(connect_socket, result->ai_addr, result->ai_addrlen);
	if (iResult == SOCKET_ERROR) 
	{
		DWORD dwError = WSAGetLastError();
		std::cout << "Unable to connect to Server " << std::endl;
		std::cout << FormatLastError(dwError, szError) << std::endl;
		closesocket(connect_socket);
		freeaddrinfo(result);
		WSACleanup();
		return;
	}
	CreateThread(NULL, 2048, (LPTHREAD_START_ROUTINE)getMessages, (LPVOID)&connect_socket, 0, NULL);

	//5) Отправка и получение данных:
	SetConsoleCP(1251);
	std::cout << "Введите ваше имя: ";
	CHAR sendbuffer[BUFFER_LENGTH] = "My Nick";
	std::cin.getline(sendbuffer, BUFFER_LENGTH);
	if (!strcmp(sendbuffer, "")) 
	//strcmp - сравнивает две строки на совпадение, 
	//или если одна строка длинее другой, то какая из:
	//положительное, больше 0 - первая строка больше, втоорой
	//отрицательное, меньше 0 - вторая строка больше, первой
	{
		CONST INT SIZE = 256;
		CHAR host[SIZE] = {};
		int res = gethostname(host, SIZE);
		if (res == 0) 
		{
			getaddrinfo(host, PORT, &hinsts, &result);
			//getaddrinfo — это версия функции в стандарте ANSI, которая обеспечивает независимый от 
			//протокола перевод имени хоста в адрес. 
			sockaddr_in* addrIn = (sockaddr_in*)result->ai_addr;
			//sockaddr_in* - структура содержащая ip адрес
			CHAR ipAddr[256];
			inet_ntop(AF_INET, &(result->ai_addr), sendbuffer, BUFFER_LENGTH);
			//ai_addr - содержит адрес сокета, то есть ip-адрес хоста (компьютера), ia_addr относится 
			//к семейству AF_NET (IPv4), указателю структуры addrinfo*
		}
	}
	do
	{
		SetConsoleCP(1251);
		//Меняем кодировку на кирилицу
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
			std::cout << FormatLastError(WSAGetLastError(), szError) << std::endl;
			std::cout << "Send failed:\t" << WSAGetLastError() << std::endl;
			closesocket(connect_socket);
			freeaddrinfo(result);
			WSACleanup();
			return;
		}
		std::cout << "Bytes sent: " << iResult << std::endl;
		ZeroMemory(sendbuffer, BUFFER_LENGTH);
		std::cin.getline(sendbuffer, BUFFER_LENGTH);
		SetConsoleCP(866);
		//Меняем кодировку на латиницу
	} while (strcmp(sendbuffer, "exit") != 0);

	iResult = shutdown(connect_socket, SD_BOTH);
	if (iResult == SOCKET_ERROR)
	{
		std::cout << FormatLastError(WSAGetLastError(), szError) << std::endl;
		std::cout << "Shutdown failed: " << WSAGetLastError() << std::endl;
	}
	closesocket(connect_socket);
	freeaddrinfo(result);
	WSACleanup();
}