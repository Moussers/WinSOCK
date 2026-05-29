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
        return "WSAETOOMANYREFS: Too many references.";
        break;
    case WSAETIMEDOUT:
        return "WSAETIMEDOUT: Connection timed out.";
        break;
    case WSAECONNREFUSED:
        return "WSAECONNREFUSED: Connection refused.";
        break;
    case WSAELOOP:
        return "WSAELOOP: Cannot translate name.";
        break;
    case WSAENAMETOOLONG:
        return "WSAENAMETOOLONG: Name too long.";
        break;
    case WSAEHOSTDOWN:
        return "WSAEHOSTDOWN: Host is down.";
        break;
    case WSAEHOSTUNREACH:
        return "WSAEHOSTUNREACH: No route to host.";
        break;
    case WSAENOTEMPTY:
        return "WSAENOTEMPTY: Directory not empty.";
        break;
    case WSAEPROCLIM:
        return "WSAEPROCLIM: Too many process.";
        break;
    case WSAEUSERS:
        return "WSAEUSERS: Use quota exceeded.";
        break;
    case WSAEDQUOT:
        return "WSAEDQUOT: Disk quota exceeded.";
        break;
    case WSAESTALE:
        return "WSAESTALE: Stale file handle reference.";
        break;
    case WSAEREMOTE:
        return "WSAEREMOTE: Item is remote.";
        break;
    case WSASYSNOTREADY:
        return "WSASYSNOTREADY: Network subsystem is unavailable.";
        break;
    case WSAVERNOTSUPPORTED:
        return "WSAVERNOTSUPPORTED: Winsock.dll version out of range.";
        break;
    case WSANOTINITIALISED:
        return "WSANOTINITIALISED: Succesful WSAStartup not yet performed.";
        break;
    case WSAEDISCON:
        return "WSAEDISCON: Graceful shutdown in progress";
        break;
    case WSAENOMORE:
        return "WSAENOMORE: No more results.";
        break;
    case WSAECANCELLED:
        return "WSAECANCELLED: Call has been canceled.";
        break;
    case WSAEINVALIDPROCTABLE:
        return "WSAEINVALIDPROCTABLE: Procedure call table is invalid.";
        break;
    case WSAEINVALIDPROVIDER:
        return "WSAEINVALIDPROVIDER: Service provider is invalid.";
        break;
    case WSAEPROVIDERFAILEDINIT:
        return "WSAEPROVIDERFAILEDINIT: Service provider failed to initialize.";
        break;
    case WSASYSCALLFAILURE:
        return "WSASYSCALLFAILURE: System call failure.";
        break;
    case WSASERVICE_NOT_FOUND:
        return "WSASERVICE_NOT_FOUND: Sevice not found.";
        break;
    case WSATYPE_NOT_FOUND:
        return "WSATYPE_NOT_FOUND: Class type not found.";
        break;
    case WSA_E_NO_MORE:
        return "WSA_E_NO_MORE: No more results.";
        break;
    case WSA_E_CANCELLED:
        return "WSA_E_CANCELLED: Call was canceled.";
        break;
    case WSAEREFUSED:
        return "WSAEREFUSED: Database query was refused.";
        break;
    case WSAHOST_NOT_FOUND:
        return "WSAHOST_NOT_FOUND: Host not found.";
        break;
    case WSATRY_AGAIN:
        return "WSATRY_AGAIN: Nonauthoritative host not found.";
        break;
    case WSANO_RECOVERY:
        return "WSANO_RECOVERY: This is a nonrecoverable error.";
        break;
    case WSANO_DATA:
        return "WSANO_DATA: Valid name, no data record of requested type.";
        break;
    case WSA_QOS_RECEIVERS:
        return "WSA_QOS_RECEIVERS: QoS receivers.";
        break;
    case WSA_QOS_SENDERS:
        return "WSA_QOS_SENDERS: No QoS sender.";
        break;
    case WSA_QOS_NO_SENDERS:
        return "WSA_QOS_NO_SENDERS: No QoS senders.";
        break;
    case WSA_QOS_NO_RECEIVERS:
        return "WSA_QOS_NO_RECEIVERS: QoS no recievers.";
        break;
    case WSA_QOS_REQUEST_CONFIRMED:
        return "WSA_QOS_REQUEST_CONFIRMED: QoS request confirmed.";
        break;
    case WSA_QOS_ADMISSION_FAILURE:
        return "WSA_QOS_ADMISSION_FAILURE: QoS admission error.";
        break;
    case WSA_QOS_POLICY_FAILURE:
        return "WSA_QOS_POLICY_FAILURE: QoS admission error.";
        break;
    case WSA_QOS_BAD_STYLE:
        return "WSA_QOS_BAD_STYLE: QoS bad style.";
        break;
    case WSA_QOS_BAD_OBJECT:
        return "WSA_QOS_BAD_OBJECT: QoS bad object.";
        break;
    case WSA_QOS_TRAFFIC_CTRL_ERROR:
        return "WSA_QOS_TRAFFIC_CTRL_ERROR: QoS traffic control error.";
        break;
    case WSA_QOS_GENERIC_ERROR:
        return "WSA_QOS_GENERIC_ERROR: QoS generic error.";
        break;
    case WSA_QOS_ESERVICETYPE:
        return "WSA_QOS_ESERVICETYPE: QoS service type error.";
        break;
    case WSA_QOS_EFLOWSPEC:
        return "WSA_QOS_EFLOWSPEC: QoS flowspec error.";
        break;
    case WSA_QOS_EPROVSPECBUF:
        return "WSA_QOS_EPROVSPECBUF: Invalid QoS provider buffer.";
        break;
    case WSA_QOS_EFILTERSTYLE:
        return "WSA_QOS_EFILTERSTYLE: Invalid QoS fulter style.";
        break;
    case WSA_QOS_EFILTERTYPE:
        return "WSA_QOS_EFILTERTYPE: Invalid QoS filter type.";
        break;
    case WSA_QOS_EFILTERCOUNT:
        return "WSA_QOS_EFILTERCOUNT: Incorrect QoS filter count.";
        break;
    case WSA_QOS_EOBJLENGTH:
        return "WSA_QOS_EOBJLENGTH: Invalid QoS object length.";
        break;
    case WSA_QOS_EFLOWCOUNT:
        return "WSA_QOS_EFLOWCOUNT: Incorrect QoS flow count.";
        break;
    case WSA_QOS_EUNKOWNPSOBJ:
        return "WSA_QOS_EUNKOWNPSOBJ: Unrecognized QoS object.";
        break;
    case WSA_QOS_EPOLICYOBJ:
        return "WSA_QOS_EPOLICYOBJ: Invalid QoS policy object.";
        break;
    case WSA_QOS_EFLOWDESC:
        return "WSA_QOS_EFLOWDESC: Invalid QoS flow descriptor.";
        break;
    case WSA_QOS_EPSFLOWSPEC:
        return "WSA_QOS_EPSFLOWSPEC: Invalid QoS provider-specific ";
        break;
    case WSA_QOS_EPSFILTERSPEC:
        return "WSA_QOS_EPSFILTERSPEC: Invalid QoS provider-specific filterspec.";
        break;
    case WSA_QOS_ESDMODEOBJ:
        return "WSA_QOS_ESDMODEOBJ: Invalid QoS shape discard mode object.";
        break;
    case WSA_QOS_ESHAPERATEOBJ:
        return "WSA_QOS_ESHAPERATEOBJ: Invalid QoS shaping rate object.";
        break;
    case WSA_QOS_RESERVED_PETYPE:
        return "WSA_QOS_RESERVED_PETYPE: Reserved policy QoS element type.";
        break;
    default: 
    {
        return "Unknow error.";
    }
    }
}