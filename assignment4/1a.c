#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<unistd.h>
#include<sys/ipc.h>
#include<signal.h>

#define KEY1 1234
#define SIZE 4096

	struct sigaction act1,act2;
	sigset_t s1,s2;
	int status=0,count=0;
void hndlr(int signo)
{
	int ret1;
	
	while(1){	
			ret1=waitpid(-1,&status,0);
			if(ret1<0)break;
			if(ret1==0)break;
			if(ret1>0)count++;
		}
}
int main()
{
	int shmid,ret,i=0,wcnt,rcnt;
	void *fvadd;
	
	shmid=shmget(KEY1,3*SIZE,0600|IPC_CREAT);
	if(shmid<0){perror("error in creating SHM object\n");exit(1);}
	//creating a shared object of 3 pagesize long with read write permission

	fvadd=shmat(shmid,0,0);
	if(fvadd<0){perror("attach could not be successfull\n");exit(2);}
	//attaching the calling process to the shared memory object just created
	
	printf("Enter the text...\n");
	
	rcnt=read(STDIN_FILENO,fvadd,3*SIZE);
	if(rcnt<0){perror("Error in reading\n");exit(3);}
	else{
	printf("%d characters were read from SHM area\n",rcnt);
	}

ret=fork();
	if(ret<0){perror("error in creation of child\n");exit(4);}

	if(ret==0)
	{	
		printf("This is child...pid %d and ppid is..%d\n",getpid(),getppid());
		 wcnt=write(STDOUT_FILENO,fvadd,3*SIZE);
	if(wcnt<0){perror("error in writing\n");exit(5);}
	else{
        printf("%d characters were written to SHM area of %d bytes\n",rcnt,wcnt);}
	exit(0);	
	}
	if(ret>0)
	{
		printf("this is parent...pid..%d\n",getpid());
	sigfillset(&s1);
	sigdelset(&s1,SIGCHLD);
	sigprocmask(SIG_SETMASK,&s1,&s2);

	act1.sa_handler=hndlr;
	sigfillset(&act1.sa_mask);

	sigaction(SIGCHLD,&act1,&act2);
	
	sigsuspend(&s1);
	}

	printf("%d Child terminated succesfully\n",count);
return 0;
}
