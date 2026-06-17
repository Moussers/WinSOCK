#include "FormatLastError.h"

LPSTR FormatLastError(DWORD dwError, CHAR szBuffer[])
{
	LPSTR lpBuffer = NULL;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, dwError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPSTR)&lpBuffer, 0, NULL);
	//FORMAT_MESSAGE_ALLOCATE_BUFFER — просит систему выделить память для буфера автоматически.
	//В этом случае lpMsgBuf должен быть указателем на void*, куда будет записан адрес выделенной
	//памяти.
	sprintf(szBuffer, "%i: %s", dwError, lpBuffer);
	LocalFree(lpBuffer);
	return szBuffer;
}