#include<stdio.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#define KEY 2222

typedef struct shmarea {

	unsigned long count;
}shmarea;

shmarea *shma;

union uni {
	int val;
	unsigned short *array;
};

struct sembuf sb1;
//unsigned short int ary[1];
	

int main()
{
	int shm_id1,sem_id2;
	int status,ret,ret1,i=0;
	union uni u1;
	unsigned long int j=0;
	unsigned short  ary[1];


	shm_id1=shmget(KEY,4096,IPC_CREAT | 0600);
	if(shm_id1<0) {perror("Error in creating shm objet\n");exit(0);}

	shma=shmat(shm_id1,0,0);
	if((signed int)shma==-1){perror("Error in attaching process \n");exit(1);}

	printf("Address of shared memory area is %x\n",shma);

	shma->count=333;

	//creating semaphore object

	sem_id2=semget(KEY,1,IPC_CREAT | 0600);
	if(sem_id2<0){perror("Error in creating semaphore object set\n");exit(2);}
//..............................................................setting semaphore value
	u1.val=1;
	ret1=semctl(sem_id2,0,SETVAL,u1);
	if(ret1<0) { perror("Error in semctl setting"); exit(3); }

//...............................................................getting semaphore value
	u1.array=ary;
	ret1=semctl(sem_id2,0,GETALL,u1);
	printf("Value of semaphore object is %d\n",ary[0]);
	if(ret1<0) { perror("error in semctl getting"); exit(4); }

while(i++<1)
{
	ret=fork();

	if(ret<0){ perror("Error in creating chid\n");exit(5);}

	if(ret>0){ printf("In parent pid is %d\n",getpid());

	while(j++<6000007)
	{
		sb1.sem_num=0;
		sb1.sem_op=-1;
		sb1.sem_flg=0;
		semop(sem_id2,&sb1,1);

		shma->count++;

		sb1.sem_num=0;
		sb1.sem_op=+1;
		sb1.sem_flg=0;
		semop(sem_id2,&sb1,1);

	}
	continue;
	}//end of parent body

	if(ret==0)
	{
		printf("In child...pid=%d...ppid=%d\n",getpid(),getppid());
		
		u1.array=ary;
		 ret1=semctl(sem_id2,0,GETALL,u1);
		  printf("Value of semaphore object in child is %d\n",ary[0]);

		  if(ret1<0) { perror("error in semctl setting"); exit(6); }


		while(j++<6000007)
		{
			 sb1.sem_num=0;
		       sb1.sem_op=-1;
		       sb1.sem_flg=0;
			semop(sem_id2,&sb1,1);

		       shma->count--;

		        sb1.sem_num=0;
		        sb1.sem_op=+1;
		        sb1.sem_flg=0;
			semop(sem_id2,&sb1,1);

		}
		exit(0);
	}// end of child body
}//end of first while

if(ret>0)
{
	while(1)
	{
		ret= waitpid(-1,&status,0);
		{
		if(ret>0)
			{
			if(WIFEXITED(status))
			   { 	if(WEXITSTATUS(status)==0)
				printf("Terminated normally and succesfully\n");
				else
				printf("Terminated Normally but not succesfull\n");   
			   }
			else
				printf("Terminated abnormally\n");
			
			}
		if(ret<0){
			printf("Final value of shared counter is %d\n",shma->count);
			exit(0);
			}

		}
	}//end of while	

}// end of if

return 0;
}

