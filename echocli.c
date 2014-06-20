#include "std.h"
#include "misc.h"

void strProcess(FILE *fp,int fd);

int main(int argc,char **argv)
{
	if(2 != argc)
	{
		fprintf(stderr,"usage:%s <ipaddress>\n",argv[0]);
		exit(1);
	}

	int iConnSockfd;
	struct sockaddr_in srvAddr;
	bzero(&srvAddr,sizeof(srvAddr));
	srvAddr.sin_family = AF_INET;
	srvAddr.sin_port = htons(8888);
	if(inet_aton(argv[1],&srvAddr.sin_addr) == 0)
	{
		fprintf(stderr,"%s <dotted-address>\n",argv[0]);
		exit(1);
	}

	//int iConnSockfd[5];
	//int i;

	//for(i = 0;i < 5; i ++)
	{
	if((iConnSockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		perror("socket");
		exit(1);
	}

	if(connect(iConnSockfd,(struct sockaddr*)&srvAddr,sizeof(srvAddr)) < 0)
	{
		perror("connect");
		exit(1);
	}
	}

	signal(SIGPIPE,SIG_IGN);

	strProcess(stdin,iConnSockfd);

	exit(0);
}

void strProcess(FILE *fp,int fd)
{
	int nwrite,nread,len;
	const int BUFF_LEN = 1024;
	char sendBuff[BUFF_LEN],recvBuff[BUFF_LEN];

	while(fgets(sendBuff,BUFF_LEN,fp) != NULL)
	{
		len = strlen(sendBuff);
		nwrite = writen(fd,sendBuff,strlen(sendBuff));
		printf("getstr:%s,strlen:%d,nwrite:%d\n",sendBuff,len,nwrite);
#if 1
		nread = readline(fd,recvBuff,BUFF_LEN);
		if(0 == nread)
		{
			printf("peer close socket!\n");
		}
#else
		nread = read(fd,recvBuff,BUFF_LEN);
#endif
		//printf("nread:%d\n",nread);
		fputs(recvBuff,stdout);
	}
}
