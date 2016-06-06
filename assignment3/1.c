#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<signal.h>
#include<unistd.h>
struct sigaction act1,act2;
sigset_t s1,s2;
void sighdl1(int signo)
{
	printf("dummy sigal 1\n");
}
void sighdl2(int signo)
{
	printf("dummy signal 2\n");
}
void sighld3(int signo)
{
	printf("dummy signal 3 for sigkill\n");
}
int main(int argc,char *argv[])
	
{
	int ret;
/*	if(argc!=2)
	{
		printf("error\n");
		exit(0);
	}
*/
	sigfillset(&s1);
	sigdelset(&s1,SIGQUIT);  //to reset a particular signal bit in bitmap
	//sigprocmask(SIG_SETMASK,&s1,&s2);//copying the bitmap to the pd of process
	
	printf("we are blocked\n");
	//mask other signals while control passed to signal handler
//	sigfillset(&act1.sa_mask);
	act1.sa_handler=sighdl1;
	sigfillset(&act1.sa_mask);

	act1.sa_flags=0;
	sigaction(SIGQUIT,&act1,&act2);
	 
								// 2nd signal handling occurs here
	
//	sigfillset(&s1);
	sigdelset(&s1,SIGINT);
//	sigprocmask(&s1);

	act1.sa_handler=sighdl2;
	sigfillset(&act1.sa_mask);//mask other signals while the handler is used

	act2.sa_flags=0;
	sigaction(SIGINT,&act1,&act2);
	//sigdelset(&s1, SIGTERM);
//        sigdelset(&s1, SIGINT);
//
				        			//	3rd signal handling occurs here
	sigdelset(&s1,SIGKILL);
	act1.sa_handler=sighld3;
	sigfillset(&act1.sa_mask);
	sigaction(SIGKILL,&act1,&act2);


	sigsuspend(&s1);//suspending for the unmasked signals also can load it in pd(if not loaded using sigprockmask()API )
	while(1){
		     sigdelset(&s1, SIGQUIT);
		          sigdelset(&s1, SIGINT);
			  sigdelset(&s1, SIGKILL);
			        sigsuspend(&s1);
			            }
	return 0;
}
	

	

