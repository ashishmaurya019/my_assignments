#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<unistd.h>
#include<error.h>

struct sigaction act1,act2;
sigset_t s1,s2;

void sighld(int signo)
{
	int ret;
	printf("Dummy handler for killing init process\n");
	ret=kill(1,SIGKILL);
	if(ret==0)
	printf("init is killed");
	else
	perror("Can not be killed");
}

int main(int argc,char *argv[])
{
	if(argc!=2)
	{	printf("ERROR!!!...Please enter two commands\n");
//	break;
	}
	printf("You are blocked\n");

	sigfillset(&s1);
	sigdelset(&s1,SIGTERM);

	sigprocmask(SIG_SETMASK,&s1,&s2);		
	
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
	
