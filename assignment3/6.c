#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<signal.h>
//#include<unistd.h>
#include<sys/wait.h>
struct sigaction act1,act2;
sigset_t s1,s2;
int count=0;
int status=0;
void sighdl1(int signo)
{
	int ret1;
	while(1)
	{
		ret1=waitpid(-1,&status,WNOHANG);
		 if(ret1<0)break;
		 if(ret1==0)break;
		 if(ret1>0){count++;}
	}
}

int main(int argc,char *argv[])
	
{
	
	int ret,i=0;
	if(argc!=2)
	{
		printf("error\n");
		//exit(0);
	}

	while(i++<5)
	{
		ret=fork();
		if(ret<0){perror("error in creation");exit(0);}
		if(ret>0){
			printf("I am parent...pid %d...ppid %d.... created child %d\n",getpid(),getppid(),ret);
			continue;
		}
		if(ret==0)
		{
			printf("I am child...pid %d ppid %d\n",getpid(),getppid());
		//exit(3);
		break;
		}
	}
if(ret>0)
	{	
	sigfillset(&s1);
	sigdelset(&s1,SIGCHLD);  
//	printf("we are blocked\n");
	sigprocmask(SIGCHLD,&s1,&s2);
		
	act1.sa_handler=sighdl1;
	sigfillset(&act1.sa_mask);
	
//	sigsuspend(&s1);

	act1.sa_flags=0;
	sigaction(SIGCHLD,&act1,&act2);

	sigsuspend(&s1);
	}
	printf("%d terminated succsfully\n",i);
	return 0;

}
	

	

