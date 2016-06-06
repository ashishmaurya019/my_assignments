#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>

struct sigaction act1,act2;
sigset_t s1,s2;

void sighld(int signo)
{
	printf("My code\n");
	int a=4,b=5;
	int sum=a+b;
	printf("Sum is %d\nYou are still blocked kill using SIGKILL\n",sum);
}

int main(int argc,char *argv[])
{
	if(argc!=2)
	{	printf("ERROR!!!...Please enter two commands\n");
//	exit(0);
	}

	sigfillset(&s1);
	sigdelset(&s1,SIGTERM);

	sigprocmask(SIG_SETMASK,&s1,&s2);		
	
	printf("You are blocked use SIGTERM for further working\n");

	act1.sa_handler=sighld;
	sigfillset(&act1.sa_mask);

	act1.sa_flags=0;	
	sigaction(SIGTERM,&act1,&act2);

//	sigsuspend(&s1);
//	printf("You are blocked\n");

	while(1)
	{
		sigdelset(&s1,SIGTERM);
	//	sigdelset(&s1,SIGINT);
		sigsuspend(&s1);
	}

	return 0;
}
	
