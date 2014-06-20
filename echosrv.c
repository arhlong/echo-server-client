#include "std.h"
#include "misc.h"

void childProcess(int fd);
void sigChild(int signo);

int main(int argc,char **argv)
{
	int iLisSockfd;
	struct sockaddr_in localAddr;
	bzero(&localAddr,sizeof(localAddr));
	localAddr.sin_family = AF_INET;
	localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	localAddr.sin_port = htons(8888);

	if((iLisSockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		perror("socket");
		exit(0);
	}

	if(bind(iLisSockfd,(struct sockaddr *)&localAddr,sizeof(localAddr)) < 0)
	{
		perror("bind");
		exit(0);
	}
	
	if(listen(iLisSockfd,5) < 0)
	{
		perror("listen");
		exit(0);
	}

	signal(SIGCHLD,sigChild);

	int iConnSockfd,iPeerAddrLen;
	struct sockaddr peerAddr;
	bzero(&peerAddr,sizeof(peerAddr));
	iPeerAddrLen = 1;
	pid_t chilPid;

	while(1)
	{
		iConnSockfd = accept(iLisSockfd,(struct sockaddr*)&peerAddr,&iPeerAddrLen);
		if(iConnSockfd < 0)
		{
			if(errno == EINTR)
			{
				perror("accept");
				continue;	
			}
			else
			{
				perror("accept");
				exit(1);
			}
		}
		if((chilPid = fork()) == 0)
		{
			close(iLisSockfd);
			childProcess(iConnSockfd);
			exit(0);
		}

		printf("child progres pid:%d\n",chilPid);
		close(iConnSockfd);
	}

	eixt(0);
}

void childProcess(int fd)
{
	int n,l;
	const int BUFF_LEN = 128;
	char recvBuff[BUFF_LEN];

again:

	while((n = read(fd,recvBuff,BUFF_LEN)) > 0)
	{
		l = writen(fd,recvBuff,n);
		//printf("recv:%s,recv len:%d,send len:%d\n",recvBuff,n,l);
	}

	if(n < 0 && errno == EINTR)
	{
		perror("read");
		goto again;
	}
	else if(n < 0)
	{
		perror("read");
	}
}

void sigChild(int signo)
{
	pid_t pid;
	int stat;
#if 0
	pid = wait(&stat);
	printf("child pid:%d exit.\n",pid);
#else
	while((pid = waitpid(-1,&stat,WNOHANG)) > 0)
	{
		printf("child progres exit.pid:%d\n",pid);
	}
#endif	
	return ;
}
