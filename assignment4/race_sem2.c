#include<sys/types.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/sem.h>

#define KEY1 1111


struct shmarea {

  unsigned long count;

}; 
union semun { //used with semctl() system call for initializing 
              //getting the current semaphore count values
  int val;                //this field is  used for SETVAL
  unsigned short *array;  //this field is used for SETALL 
  // other fields are omitted

};
struct shmarea *shma;

struct sembuf sb1,sb2,sb3,sb4[3];
int main()
{

   
  char value,id;
  int ret,ret1,ret2,status, shm_id1,sem_id2;
  unsigned long int i=0,j=0;
  unsigned short ary1[1],ary[] = {1};
  union semun u1;
   shm_id1 =  shmget(KEY1,4096,IPC_CREAT|0600);
   if(shm_id1<0) { perror("error in shmget"); exit(1); }
 
   
   shma = shmat(shm_id1,0,0);  
   if((signed int)shma == -1) { perror("error in shmat"); exit(2); }


  printf("pointer value is %x\n", shma); 


   shma->count = 3333; 
   
   //Create semaphore
   sem_id2 = semget(KEY1,1,IPC_CREAT | 0600);//with read/write permissions 
   
   u1.val = 1; //initialize semaphore value(s)
               ///setting the array ptr in the union
   ret1 = semctl(sem_id2,0,SETVAL,u1);//setting the semaphore values
   //u1.array = ary; //setting the array ptr in the union
   //ret1 = semctl(sem_id2,0,SETALL,u1);//setting the semaphore values

   if(ret1<0) { perror("error in semctl setting"); exit(4); }

   u1.array = ary1;
 
  ret1 = semctl(sem_id2,0,GETALL,u1);//getting the semaphore values
  if(ret1<0) { perror("error in semctl getting"); exit(5); }


  printf("the actual values of the semaphores in parent= %d\n",ary1[0]); 
  if(sem_id2<0)
  {

                perror("error in semget");
                exit(2);
  }
  while(i++<1)
  { 

   ret = fork();
 
   if(ret<0){ 
           perror("error in fork"); 
           printf("the final value of i is %lu\n", i);
          
           //an example for normal termination, but not successful 
           exit(1); 
   }

   if(ret>0)
   { 
	   printf("I am in parent process context\n"); 
           printf("in parent .. ppid is %lu ...and pid is %lu\n", 
		   getppid(),getpid());	   
	   
           
		while(j++ < 600000)
		{
			 sb1.sem_num = 0;  //semaphore instance no.
    			 sb1.sem_op = -1;  //decrement operation 
    			 sb1.sem_flg = 0;  //not using flags
		         semop(sem_id2,&sb1,1);//check for errors   
			 
                         shma->count++;     //ok
			 
                         sb1.sem_num = 0;  //we are operating on the second semaphore
		         sb1.sem_op = +1;  //increment
    			 sb1.sem_flg = 0;
			 semop(sem_id2,&sb1,1); 
		}

         continue;
   }

   if(ret==0)
   { 
	   printf("I am in child process context\n"); 
           printf("in child .. ppid is %lu ...and pid is %lu\n", 
		   getppid(),getpid());	   
	   
  	   u1.array = ary1;
 
           ret1 = semctl(sem_id2,0,GETALL,u1);//getting the semaphore values


  	   if(ret1<0) { perror("error in semctl getting"); exit(5); }


  	   printf("the actual values of the semaphores are in child=  %d\n", \
                    ary1[0]); 
          
           while(j++<600000)
	   {		
		 sb1.sem_num = 0;  //semaphore instance no.
    		 sb1.sem_op = -1;  //decrement operation 
    		 sb1.sem_flg = 0;
		 semop(sem_id2,&sb1,1);
		 
		 shma->count--;
		 
		 sb1.sem_num = 0;  //we are operating on the second semaphore
		 sb1.sem_op = +1;  //increment
    		 sb1.sem_flg = 0;
		 semop(sem_id2,&sb1,1);
	   }
           
           exit(0); 
   }

 }//while

 

 if(ret>0)
 {
   while(1){ 
    ret = waitpid(-1,&status,0);
    if(ret>0){

    if(WIFEXITED(status))     //normal termination of the process
    {
       if(WEXITSTATUS(status) == 0){ //normal and successfull
       }
       else{
       }
    }
    else{ 
    }
   }//ret>0
    //we may use break instead of exit(0) 
    if(ret<0) {

           //you may destroy the semaphores and shared memory objects 

           printf("final value of shared counter is %d\n", shma->count); 
           exit(0); 

    } 
   } //second while 
  }//if after while loop 

return 0;
   
}


