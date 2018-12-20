#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define MAXPENDING 5
#define MAXUSER 2


int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock[MAXUSER];
	unsigned int uiUser;
	int tempSock;
	int iMaxSock;
	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	unsigned short echoServPort;
	unsigned int clntLen;
	int iRet;
	unsigned char buf[500];
	fd_set fs_status;
	int iCnt;
	int iCnt2;

	echoServPort = 9999;

	serv_sock = socket(AF_INET,SOCK_STREAM, 0);

	memset(&serv_addr, 0 , sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(echoServPort);

	if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
		error_handling("bind error");

	if(listen(serv_sock,10)==-1)
		error_handling("listen error");

	clntLen = sizeof(clnt_addr);
	
	iMaxSock = serv_sock;
	uiUser = 0;
	
	while(1){

		FD_ZERO(&fs_status);
		FD_SET(0, &fs_status);
		FD_SET(serv_sock,&fs_status);

		for(iCnt2= uiUser; iCnt2>0; --iCnt2)
		{
			FD_SET(clnt_sock[iCnt2-1], &fs_status);
			if(clnt_sock[iCnt2-1] >= iMaxSock)
			{
				iMaxSock = clnt_sock[iCnt2-1]+1;
			}
		}

		iRet = Select (iMaxSock, &fs_status, 0, 0, 0);

		if(iRet < 0)
		{
			for(iCnt = uiUser; iCnt>0; --iCnt)
			{
				write(clnt_sock[iCnt-1], "q", 2);
			}
			break;
		}

		if(FD_ISSET(servSock, &fs_status)==1)
		{
			tmepSock= accpet(servSock,(struct sockaddr *)&clnt_addr, &clntLen);
			if(tempScok < 0)
			{
				printf("accept error\n");
				continue;}

			printf("%d connected\n", inet_ntoa(clnt_addr.sin_addr));
			if(MAXUSER <= uiUser)
			{
				close(tempSock);
				continue;
			}
			clnt_sock[uiUsier] = tempSock;
			++uiUser;
			printf(" %d\n",uiUser);
		}
		else
		{
			for(iCnt = uiUser; iCnt>0; --iCnt)
			{
				if (FD_ISSET(clnt_sock[iCnt -1], &fs_status)==1)
				{
					iRet = read(clnt_sock[iCnt-1], buf, 499);
					printf(" clint : %d", iCnt-1);
					fflush(stdout);
					write(1, buf, iRet-1);
					printf("\n");

					for(iCnt2=uiUser; iCnt2>0;--iCnt2){
						write(clnt_sock[iCnt2-1],buf,iRet);
					}
				}
			}
		}
		if ('q' == buf[0])
		{
			break;
		}

	}
	close(serv_sock);
	for(iCnt = uiUser ; iCnt2>0 ; iCnt2--)
	{
		close(clnt_sock[iCnt2-1];
	}
	return 0;
	}
