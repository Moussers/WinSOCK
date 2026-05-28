// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <WinSock2.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

CONST CHAR* FormatWSALastError() {
//CHAR* - звездочка указатель на массив
//__declspec - метка которая обозначает что функцию можно экспортировать как dll библиотеку
    int err = WSAGetLastError();
    switch (err) 
    {
    case WSA_INVALID_HANDLE:
        return "WSA_INVALID_HANDLE: Specifed event object handle is invalid.";
        break;
    case WSA_NOT_ENOUGH_MEMORY:
        return "WSA_NOT_ENOUGH_MEMORY: Insufficient memory available.";
        break;
    case WSA_INVALID_PARAMETER:
        return "WSA_INVALID_PARAMETER: One or more parameters are invalid.";
        break;
    case WSA_OPERATION_ABORTED:
        return "WSA_OPERATION_ABORTED: Ovarlapped operation aborted.";
        break;
    case WSA_IO_INCOMPLETE:
        return "WSA_IO_INCOMPLETE: Overlapped operations will complete later.";
        break;
    case WSAEINTR:
        return "WSAEINTR: Interrupted function call.";
        break;
    case WSAEBADF:
        return "WSAEBADF: File handle is not valid.";
        break;
    case WSAEACCES:
        return "WSAEACCES: Permission denied.";
        break;
    case WSAEFAULT:
        return "WSAEFAULT: Bad address.";
        break;
    case WSAEINVAL:
        return "WSAEINVAL: Invalid argument.";
        break;
    case WSAEMFILE:
        return "WSAEMFILE: Too many open files.";
        break;
    case WSAEWOULDBLOCK:
        return "WSAEWOULDBLOCK: Resource temporily unavailable";
        break;
    case WSAENOTSOCK:
        return "WSAENOTSOCK: Socket operation on nonsocket";
        break;
    case WSAEINPROGRESS:
        return "WSAEINPROGRESS: Operation in now progress.";
        break;
    case WSAEALREADY:
        return "WSAEALREADY: Operation now in progress.";
        break;
    case WSAEDESTADDRREQ:
        return "WSAEDESTADDRREQ: Destination address required.";
        break;
    case WSAEMSGSIZE:
        return "WSAEMSGSIZE: Message too long.";
        break;
    case WSAEPROTOTYPE:
        return "WSAEPROTOTYPE: Protocol wrong type for socket.";
        break;
    case WSAENOPROTOOPT:
        return "WSAENOPROTOOPT: Bad protocol option.";
        break;
    case WSAEPROTONOSUPPORT:
        return "WSAEPROTONOSUPPORT: Protocol no supported.";
        break;
    case WSAEOPNOTSUPP:
        return "WSAEOPNOTSUPP: Operation not supported.";
        break;
    case WSAEPFNOSUPPORT:
        return "WSAEPFNOSUPPORT: Protocol family not suported.";
        break;
    case WSAEAFNOSUPPORT:
        return "WSAEAFNOSUPPORT: Address family not supported by protocol family.";
        break;
    case WSAEADDRINUSE:
        return "WSAEADDRINUSE: Address already in use";
        break;
    case WSAEADDRNOTAVAIL:
        return "WSAEADDRNOTAVAIL: Cannot assing requested address";
        break;
    case WSAENETDOWN:
        return "WSAENETDOWN: Network is down.";
        break;
    case WSAENETUNREACH:
        return "WSAENETUNREACH: Network is unreachable.";
        break;
    case WSAENETRESET:
        return "WSAENETRESET: Network dropped connection on reset.";
        break;
    case WSAECONNRESET:
        return "WSAECONNRESET: Connection reset by peer.";
        //peer - переводится как пользователь, к примеру сеть P2P - Peer To Peer (одноранговая/децентрализованная сеть) 
        break;
    case WSAENOBUFS:
        return "WSAENOBUFS: No buffer space available.";
        break;
    case WSAEISCONN:
        return "WSAEISCONN: Socket is not connected.";
        break;
    case WSAESHUTDOWN:
        return "WSAESHUTDOWN: Cannot send after socket shutdown.";
        break;
    case WSAETOOMANYREFS:
        return "";
        break;
    }
}