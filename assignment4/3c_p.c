#include<stdio.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>
#include<sys/wait.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>
#define KEY 2223

typedef struct shm_area {

  unsigned short rd_index;  
  unsigned short wr_index;  
  unsigned short buf_size_max; 
  unsigned int used_slot_count;	//no elements used in the circular buffer 
  char st[50][256];   
}shmarea;

shmarea *shma;

union uni {
	int val;
	unsigned short *array;
};

struct sembuf sb_array[3];
//unsigned short int ary[1];
	

int main()
{
	int shm_id1,sem_id2;
	int status,ret,ret1,i=0;
	union uni u1;
	unsigned long int j=0;
	unsigned short  ary[3],ary1[]={50,1,0};
	

	shm_id1=shmget(KEY,sizeof(shmarea),IPC_CREAT | 0600);
	if(shm_id1<0 && errno != EEXIST) {perror("Error in creating shm objet\n");exit(0);}

	shma=shmat(shm_id1,0,0);
	if((signed int)shma==-1){perror("Error in attaching process \n");exit(1);}

	printf("Address of shared memory area is %x\n",shma);


	//creating semaphore object

	sem_id2=semget(KEY,3,IPC_CREAT | 0600);
	
	if(sem_id2<0){perror("Error in creating semaphore object set\n");exit(2);}
	
//..............................................................setting semaphore value
	u1.array=ary1;
	ret1=semctl(sem_id2,0,SETALL,u1);
//	printf("Initial Value of semaphore objects is ob1=%d,ob2=%d,ob3=%d\n",ary1[0],ary1[1],ary1[2]);
	if(ret1<0 ) { perror("Error in semctl setting"); exit(3); }



        //all initializations must complete here ???

		shma->wr_index = 0;
		shma->rd_index = 0; 
  		shma->buf_size_max=50;
  		shma->used_slot_count = 0; 

while(i++<2)
{
	ret=fork();

	if(ret<0){ perror("Error in creating chid\n");exit(5);}

//parent starts here......................................................

	if(ret>0)
	{ 	
		printf("In parent...parent pid is %d\n",getpid());
	continue;
	}	
// parent end....................................................

//child start...........................................................	
  	if(ret==0)
	{
		char strs[256];
	   if(i==1) //first child.......................................................
	   {
		printf("In First child(Producer)...pid=%d...ppid=%d\n",getpid(),getppid());

		u1.array=ary;
		ret1=semctl(sem_id2,0,GETALL,u1);
		printf("Initial Value of semaphore objects is ob1=%d,ob2=%d,ob3=%d\n",ary[0],ary[1],ary[2]);
		if(ret1<0) { perror("error in semctl getting"); exit(4); }
	
  		
//producer
		while(1)
		{
			printf("Producer starts here....\n");
		
			sb_array[0].sem_num = 0;//preparing the decrement operation on 
    			sb_array[0].sem_op = -1;//semaphore 0 in the sem.object -meaning, use a free slot in producer                           		
    			sb_array[0].sem_flg = 0;
		
		
    			sb_array[1].sem_num = 1;   //decrement critical section semaphore 
    			sb_array[1].sem_op = -1;   //acquire semaphore lock !!!
    			sb_array[1].sem_flg = 0;
 
    			semop(sem_id2,sb_array,2);   //operation on semapore is allowed only when both the above condition is true 		

    			if(shma->used_slot_count < shma->buf_size_max)
    			{
				printf("Producer inside lock....\n");

				int ret2=read(STDIN_FILENO,strs,256);

				if(ret2<0){perror("Error in reading");exit(0);}
		
				printf("%d characters written into SHM Buffer \n",ret2);
		
				strs[ret2]='\0';					
		
       				strcpy(shma->st[shma->wr_index],strs);
					
       				shma->wr_index = (shma->wr_index+1)%shma->buf_size_max;
				shma->wr_index++;
      				shma->used_slot_count++;
				bzero(strs,256);
    			}
   
    			sb_array[1].sem_num = 1;  //critical section semaphore
    			sb_array[1].sem_op = +1;  //releasing critical section semaphore  
    			sb_array[1].sem_flg = 0;

    			sb_array[2].sem_num = 2;  //incrementing filled slots semaphore count 
    			sb_array[2].sem_op = +1;  //meaning, this semaphore maintains the count
                              		//of filled slots - this is use ful in the
                              		//context of consumer 
    			sb_array[2].sem_flg = 0;
 
    			semop(sem_id2,&sb_array[1],2);
 	//		exit(0);
		}//end of inner while
	
    	    }//end of first child body...........................................................

	    if(i==2)
	    {
			
			printf("Second child(Consumer)...pid=%d...ppid=%d\n",getpid(),getppid());
			u1.array=ary;
			int ret1 = semctl(sem_id2,0,GETALL,u1);
			printf("values of semaphores are...s1=%d,s2=%d,s3=%d\n",ary[0],ary[1],ary[2]);
			if(ret1<0){perror("error in getting values");exit(1);}
		while(1){
		//	printf("Consumer starts here...\n");
			sb_array[2].sem_num = 2;
			sb_array[2].sem_op = -1;
			sb_array[2].sem_flg = 0;
			
	//		printf("Second child(Consumer) blocks here after creation...\n");			

			sb_array[1].sem_num = 1;
			sb_array[1].sem_op = -1;
			sb_array[1].sem_flg = 0;
		if(shma->used_slot_count != 0 ){
			printf("Consumer inside lock...\n");
			int wcnt = write(STDOUT_FILENO,shma->st[shma->rd_index],256);
			if(wcnt<0){perror("error in writting...\n");exit(0);}
			else 
			printf("%d read from the SHM Buffer\n",strlen(shma->st[shma->rd_index]));
				
			shma->rd_index = (shma->rd_index +1)% shma->buf_size_max;
			shma->rd_index++;
			shma->used_slot_count--;
			}
			
			sb_array[1].sem_num = 1;
			sb_array[1].sem_op = +1;
			sb_array[1].sem_flg = 0;
			
		//	printf("");			

			sb_array[0].sem_num = 0;
			sb_array[0].sem_op = +1;
			sb_array[0].sem_flg = 0;
			
			semop(sem_id2, sb_array, 2);
	//		exit(0);
			}
	      }//end of second child
	}//end of children body..........................................................................................................
}//end of first while......................................................

// parent cleaning terminated child or waiting for unterminated child
if(ret>0)
{
	while(1)
	{
		ret= waitpid(-1,&status,0);
		{
		if(ret>0)//Normal termination
			{
			if(WIFEXITED(status))
			   { 	if(WEXITSTATUS(status)==0)
				{printf("Terminated normally and succesfully\n");exit(0);}
				else
				{printf("Terminated Normally but not succesfull\n");exit(0);}   
			   }
			else
				{printf("Terminated abnormally\n");exit(1);}
			
			}
		if(ret<0){
			printf("Nothing to clear...\n");
			exit(0);
			}

		}
	}//end of while	

}// end of if



return 0;

}

