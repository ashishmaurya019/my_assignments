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

int shmid;	
int main()
{
	int ret,i=0,wcnt,rcnt;
	void *fvadd;
	
	 shmid=shmget(KEY1,3*SIZE,0600|IPC_CREAT);
        if(shmid<0){perror("error in creating SHM object\n");exit(0);}
        //creating a shared object of 3 pagesize long with read write permission
	else{printf("shmid=%d\n",shmid);}

	fvadd=shmat(shmid,0,0);
	if(fvadd<0){perror("attach could not be successfull\n");exit(0);}
	//attaching the calling process to the shared memory object just created
	else{printf("first virtual address=%x\n",fvadd);}
	
	//printf("This is child...pid %d and ppid is..%d\n",getpid(),getppid());
	
	wcnt=write(STDOUT_FILENO,fvadd,3*SIZE);
	if(wcnt<0)
	{
	perror("error in writing\n");exit(0);
	}
	else{
        printf("%d characters read from SHM area of 12k bytes\n",wcnt);}
	exit(0);	
	
return 0;
}
