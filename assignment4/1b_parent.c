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
	else{printf("first virtual adress is %x\n",fvadd);}	

	printf("Enter the text...\n");
		
	rcnt=read(STDIN_FILENO,fvadd,3*SIZE);

	if(rcnt<0)
	{
	perror("Error in reading\n");exit(0);
	}
	else{
	printf("%d characters written into SHM area\n",rcnt);
	}

	
return 0;
}
