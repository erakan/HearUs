/**
 * HearUs ~ Announcement device application and client for DM&P's Mity-Mite Module
 * Copyright (c) 2005 Erdinc AKAN (erakan@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 **/

#include "dsock.h"
#include <dos.h>
#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


#define PRG_VERSION				".01 Beta"
#define PRG_CONFIG				"hearus.cfg"
#define PRG_PORT				(1010)
#define SocketDestroy(X)	SocketDestory(X)
#define TYRFAIL(X,Y) 		if(X == Y){printf(" Failed...\n"); return 1;}else{printf(" Succeded.\n");}
#define DEPT_FILE				"dept.txt"
#define MSG_FILE				"msg.txt"
#define PRG_DELAY				(10)
// #define BYTE unsigned char

/**************************************************************** 
**
** Struct Definitions 
**
*****************************************************************/
typedef struct _Date {
	BYTE	Day, Month, Hour, Minute, Second;
	int		Year;
}DATE;

typedef struct _tagMessageListITEM{
	struct _tagMessageListITEM * next;
	char	Message[161];
	char	Faculty[81];
	char	From[81];
	DATE	Date;
}MESSAGELISTITEM, *LPMESSAGELISTITEM;

typedef struct _tagRGB_rec
{
	BYTE red;
	BYTE green;
	BYTE blue;
}RGB_REC,*LPRGB_REC;

/**************************************************************** 
**
** Globals
**
*****************************************************************/

BYTE number[16];
RGB_REC VgaPal[16];
BYTE far *vga = (BYTE far*)0xA0000000L;
BYTE CharBuff[3596];
int textrow;
LPMESSAGELISTITEM MessageListTop = NULL;

/**************************************************************** 
**
** List Related Functions
**
*****************************************************************/

LPMESSAGELISTITEM GetLastListItem()
{
	LPMESSAGELISTITEM p;
	p = MessageListTop;
	while(p && p->next)p=p->next;
	return p;
}
/*
void AddMessageListItem(char *From, char *Faculty, char *Message, BYTE Day, BYTE Month, int Year, BYTE Hour, BYTE Minute)
{
	LPMESSAGELISTITEM pListItem, p;
	pListItem = new MESSAGELISTITEM;
	strcpy(pListItem->From, From);
	strcpy(pListItem->Message, Message);
	strcpy(pListItem->Faculty, Faculty);
	pListItem->Date.Day = Day;
	pListItem->Date.Month = Month;
	pListItem->Date.Year = Year;
	pListItem->Date.Hour = Hour;
	pListItem->Date.Minute = Minute;
	pListItem->next = NULL;
	p= GetLastListItem();
	if(p)
		p->next = pListItem;
	else
		MessageListTop = pListItem;

}

void RemoveListItem(LPMESSAGELISTITEM pli)
{
	LPMESSAGELISTITEM	p,p1;
	if(!pli) return;
	p1 = p = MessageListTop;
	if(p == pli)
	{
		MessageListTop = MessageListTop->next;
		delete p;
		return;
	}
	while(p)
	{
		if(p->next == pli)
		{
			p->next = pli->next;
			delete pli;
			break;
		}
		p= p->next;
	}
}

void ClearList()
{
	while(MessageListTop)
		RemoveListItem(MessageListTop);
}
*/

void WriteListItem(LPMESSAGELISTITEM p)
{
	FILE *file;
	char szBuff[1024];
	int len;
	file = fopen(MSG_FILE, "a");
	if(file)
	{
		fseek(file,0,SEEK_END);
		fprintf(file, "%0.2d.%0.2d.%0.4d -> %s\n%0.2d:%0.2d:%0.2d   -> Gon:%s\n           -> NOT:%s\n\n",
					p->Date.Day,p->Date.Month,p->Date.Year, p->Faculty,
					p->Date.Hour, p->Date.Minute, p->Date.Second, p->From,
					p->Message);
		fclose(file);
	}
}

/**************************************************************** 
**
** Server/Connection Functions
**
*****************************************************************/
BOOL AcceptConnections(SOCKET socket)
{
	char	szBuf[1024];
	char	smBuf[10];
	char	*szMsg;
	char	del[] = "\1";
	int	pos, nType, nOK;
	int	nLen;
	char	*p, *p2;
	BOOL	err = FALSE;
	DWORD	dwRemoteIp;
	MESSAGELISTITEM msg;

	if(SocketAccept(socket, &dwRemoteIp) == FALSE)
		return FALSE;

	inet_ntoa(szBuf, dwRemoteIp);
	SocketGetString(socket, szBuf, 1024);
	p = strtok(szBuf, del);
	if(*p== 'M' && *(p+1) =='S' && *(p+2) == 'G')
	{
		pos = 1;
		nType = 0;
		nOK = 0;
		p = strtok(NULL, del);
		if(p != NULL)
		{
			// nLen = atoi(result);
			while(p!=NULL)
			{
				if(pos==2)
				{
					switch(nType)
					{
						case 1: // Personel
							strcpy(msg.From,p);
							nOK++;
							break;
						case 2: // Fakülte
							strcpy(msg.Faculty, p);
							nOK++;
							break;
						case 3: // Yayýn Tarihi
								// iso standardýnda olmak zorunda!
								//20-02-2005 12:52:00
								smBuf[0] = *p;
								smBuf[1] = *(p+1);
								smBuf[2] = 0;
								msg.Date.Day = atoi(smBuf);
								smBuf[0] = *(p+3);
								smBuf[1] = *(p+4);
								msg.Date.Month = atoi(smBuf);
								smBuf[0] = *(p+11);
								smBuf[1] = *(p+12);
								msg.Date.Hour = atoi(smBuf);
								smBuf[0] = *(p+14);
								smBuf[1] = *(p+15);
								msg.Date.Minute = atoi(smBuf);
								smBuf[0] = *(p+6);
								smBuf[1] = *(p+7);
								smBuf[2] = *(p+8);
								smBuf[3] = *(p+9);
								smBuf[4] = 0;
								msg.Date.Year = atoi(smBuf);
							nOK++;
							break;
						case 4: // Mesaj
							strcpy(msg.Message, p);
							nOK++;
							break;
					}
					pos = 1;	
					nType=0;
				}
				else
				{
					switch(*p)
					{
						case 'P': nType=1; pos++; break;
						case 'F': nType=2; pos++; break;
						case 'D': nType=3; pos++; break;
						case 'M': nType=4; pos++; break;
					}
				}
				p = strtok(NULL, del);
			}
		}
		else
			err = TRUE;
	}
	else
		err = TRUE;
	if(!err && nOK == 4)
	{
		/*AddMessageListItem(msg.From, msg.Faculty, 
							msg.Message, msg.Date.Day,
							msg.Date.Month, msg.Date.Year,
							msg.Date.Hour, msg.Date.Minute);*/
		WriteListItem(&msg);
	}
	SocketClose(socket);
	SocketBind(socket, 0L, PRG_PORT);
	SocketListen(socket);
	return TRUE;
} 

/**************************************************************** 
**
** Graphical Interface Functions
**
*****************************************************************/
void VgaBitMask(BYTE maskno)
{
	outportb(0x03c4, 2);
	outportb(0x03c5, maskno);
}

void Screen_Open()
{
	union REGS regs;
	regs.h.ah = 0x00;
	regs.h.al = 0x12;
	int86(0x10, &regs, &regs);
}

void Screen_Close()
{
	union REGS regs;
	regs.h.ah = 0x00;
	regs.h.al = 0x03;
	int86(0x10, &regs, &regs);
}

void LoadVGAPic(char * fname, int yline)
{
	BYTE color_counter;
	BYTE dat[64];
	int i;
	struct REGPACK r;
	FILE *pfile;
	// IO exception
	pfile = fopen(fname, "rb");
	if(!pfile) return;
	Screen_Open();
	yline *=80;
	r.r_ax = 0x1009;
	r.r_es = FP_SEG(number);
	r.r_dx = FP_OFF(number);
	intr(0x10, &r);
	fread(VgaPal, 48, 1, pfile);
	memset(&r, 0, sizeof(r));
	for(color_counter =0;color_counter <16;color_counter++)
	{
		r.r_ax = 0x1010;
		r.r_bx = number[color_counter];
		r.r_dx = VgaPal[color_counter].red<<8;
		r.r_cx = (VgaPal[color_counter].green<<8) + VgaPal[color_counter].blue;
		intr(0x10, &r);
	}
	VgaBitMask(1); fread((void*)vga, yline, 1, pfile);
	VgaBitMask(2); fread((void*)vga, yline, 1, pfile);
	VgaBitMask(4); fread((void*)vga, yline, 1, pfile);
	VgaBitMask(8); fread((void*)vga, yline, 1, pfile);
	VgaBitMask(15);
	fclose(pfile);
}

void loadfont()
{
	char * fname = "coolopti.chr";
	FILE * ffile;
	ffile = fopen(fname, "rb");
	fseek(ffile, 89,0);
	fread(CharBuff,3685-89,1, ffile);
	fclose(ffile);
}

void WriteOFont(int x, int y, BYTE clr, char * str1)
{
	int j,k,l, slen;
	VgaBitMask(clr);
	while(*str1 != '\0')
	{
		k=*str1;
		for(l=0;l<14;l++)
			*(vga+y*640+x+l*80) = CharBuff[k*14+l+1];
		x++;
		str1++;
	}
	VgaBitMask(15);
}

void clearVGA(int yline)
{
	memset((void*)(vga+yline*80),0, 38400-yline*80);
	WriteOFont(0,58,9, "HearUs Module - Software by Erdinc AKAN");
}

void clearLine(int x, int y, int len)
{
	int i;
	for(i=0;i<14;i++)
		memset((void*)(vga+x+y*640+i*80), 0, len);
}

void InitGraph()
{
	LoadVGAPic("ululog2.pic",110);
	loadfont();
}
int getLine(FILE * file, char *buff)
{
	int len = 0;
	char c;
	while(!feof(file))
	{
		c = fgetc(file);
		if(c == EOF) return -1;
		if(c == '\n') break;
		*buff = c;
		buff++;
		len++;
	}
	return len;
}

void DumpDeptFile(char * fsrc)
{
	FILE *pfile;
	char st[180];
	int nlen;
	BYTE x;
	// IO exception
	pfile = fopen(fsrc, "rt");
	if(!pfile) return;
	nlen = getLine(pfile, st);
	if(nlen!= -1)
	{
		clearLine(13,11,70);
		x=35-nlen / 2;
		WriteOFont(x+10,11,14,st);
	}
	fclose(pfile);
}

void DumpTextFileCall(char * Fsrc)
{
	FILE *tfile;
	char buff[180];
	char c;
	int crow;
	BYTE clr;
	tfile = fopen(Fsrc, "rt");
	if(!tfile) return;
	clearVGA(110);
	for(crow =0;crow <textrow;crow++)
	{
		if(getLine(tfile, buff)== -1)
		{
			textrow =0;
			fseek(tfile,0,0);
			break;
		}
	}
	for(crow=0; crow<20;crow++)
	{
		memset(buff, 0, 180);
		if(getLine(tfile, buff)== -1)
		{
			textrow=0;
			break;
		}
		clr =crow%4;
		switch(clr)
		{
			case 0:
			case 1:clr = random(5)+11;break;
			case 2:
			case 3:clr =9;break;
		}
		WriteOFont(0,crow*2+15,clr,buff);
		textrow++;
	}
	fclose(tfile);
}

void InitSection()
{
  textrow=0;
  InitGraph();
}

int main(void)
{
	DWORD   dwServerIp;
	char    szServerIp[20];
	BOOL		reWrite;
	SOCKET  socket;
	clock_t	clk;

	printf("HEARUS\n");
	printf("Announcement Module Server Version %s\n", PRG_VERSION);
	printf("Copyright (C) 2005 Erdinc AKAN\n");
	printf("Initializing DSock Library...");
	TYRFAIL(DSock_Open(), FALSE);
	printf("Loading Config File %s...", PRG_CONFIG);
	TYRFAIL(DSock_LoadConfigFile(PRG_CONFIG), FALSE);
	printf("Creating Socket...");
	socket = SocketCreate(TCP_SOCKET);
	if(socket == INVALID_SOCKET)
	{
		printf(" Failed...\n");
		DSock_Close();
		return 1;
	}
	else
		printf(" Succeded.\n");
	printf("Binding Socket...");
	if(SocketBind(socket, 0L, PRG_PORT) == FALSE)
	{
		printf(" Failed...\n");
		SocketDestroy(socket);
		DSock_Close();
		return 1;
	}
	else
		printf(" Succeded.\n");
	printf("Starting to listen socket...");
	if(SocketListen(socket) == FALSE)
	{
		printf(" Failed...\n");
		SocketDestroy(socket);
		DSock_Close();
		return 1;
	}
	else
		printf(" Succeded.\n");
	dwServerIp = DSock_GetHostIp();
	inet_ntoa(szServerIp, dwServerIp);

	//TODO: Load messages from previous state!

	printf("Running in server mode waiting for connections on %s port %d...\n", szServerIp, PRG_PORT);
	delay(1000);
	InitSection();
	clk = clock();
	reWrite = TRUE;
	DumpDeptFile(DEPT_FILE);
	clearVGA(110);
	// Main Loop
	for(;;)
	{
		if(kbhit())
			break;
		if(AcceptConnections(socket))
				reWrite = TRUE;
		if((clock() - clk) >= 18.2 * PRG_DELAY)
				reWrite = TRUE;
		if(reWrite)
		{
			clk = clock();
			DumpTextFileCall(MSG_FILE);
			reWrite = FALSE;
		}
	}
	//Remove Everything (Probably these functions will never be called on the real world, sad...)
	//ClearList();
	Screen_Close();
	printf("Closing socket... ");
	SocketDestory(socket);
	printf(" Succeded!\n");
	printf("Shutting down server...");
	DSock_Close();
	printf(" Succeded!\n");
	printf("Have a nice day, and please support us ;)");
	return 0;
}