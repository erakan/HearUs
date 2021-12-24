
#ifndef _DMP_DOS_Sockets_H_
#define _DMP_DOS_Sockets_H_

/*

DM&P DSock
Copyright (C) 2001 by DM&P.

This library is for DM&P product user only

*/

typedef unsigned char  BYTE;
typedef unsigned short WORD;
typedef unsigned long  DWORD;

#ifdef __cplusplus
  enum BOOL { FALSE, TRUE };
  #define bool  BOOL
  #define false FALSE
  #define true  TRUE
#else
  typedef int BOOL;
  #define FALSE 0
  #define TRUE  1
#endif

typedef          char  SBYTE;
typedef          short SWORD;
typedef          short sint;
typedef unsigned short usint;

typedef unsigned char  uchar;
typedef unsigned int   uint;
typedef unsigned long  ulong;


#ifdef __cplusplus
extern "C" {
#endif

#define DSOCK_VERSION "0.12"

BOOL  DSock_Open();
void  DSock_Close();
BOOL  DSock_DoBootp();
BOOL  DSock_LoadConfigFile(char *szFile);
void  DSock_AddGateway(DWORD dwIp);
DWORD DSock_GetGateway();
void  DSock_AddDomainNameServer(DWORD dwIp);
DWORD DSock_GetDomainNameServer();
DWORD DSock_Resolve(char *szName);
BYTE *DSock_GetMacAddr();
DWORD DSock_GetHostIp();
void  DSock_SetHostIp(DWORD dwIp);
DWORD DSock_GetNetmask();
void  DSock_SetNetMask(DWORD dwNetmask);

char *inet_ntoa(char *sz,DWORD dw);
DWORD inet_addr(char *sz);
WORD  ntohs(WORD w);
WORD  htons(WORD w);
DWORD ntohl(DWORD dw);
DWORD htonl(DWORD dw);


typedef int SOCKET;

#define INVALID_SOCKET  (SOCKET)(0)
#define TCP_SOCKET      1
#define UDP_SOCKET      2

SOCKET SocketCreate     (int nType);
BOOL   SocketDestory    (SOCKET s);
BOOL   SocketClose      (SOCKET s);
void   SocketAbort      (SOCKET s);
BOOL   SocketBind       (SOCKET s,DWORD dwAddr,WORD wPort);
BOOL   SocketListen     (SOCKET s);
BOOL   SocketAccept     (SOCKET s,DWORD *pdwAddr);
BOOL   SocketConnect    (SOCKET s,DWORD dwAddr,WORD wPort);
int    SocketRecv       (SOCKET s,BYTE *pby,int nLen);
int    SocketRecv2      (SOCKET s,BYTE *pby,int nLen);
int    SocketSend       (SOCKET s,BYTE *pby,int nLen);
int    SocketDataReady  (SOCKET s);
void   SocketPutChar    (SOCKET s,char c);
void   SocketGetChar    (SOCKET s,char *pc);
void   SocketPutString  (SOCKET s,char *szBuf);
int    SocketGetString  (SOCKET s,char *szBuf,int nBufSize);
BOOL   SocketIsConnected(SOCKET s);

#ifdef __cplusplus
}
#endif

#endif // _DMP_DOS_Sockets_H_
