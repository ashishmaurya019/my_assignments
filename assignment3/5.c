#include <stdio.h> 
#include <string.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <signal.h> 

sigset_t s1,s2; //these are bitmaps - data types defined by operating system 

struct sigaction act1,act2; //another OS defined objects


void sig_hdl1(int signo)  //this handler is used to setup custom
                          //action for SIGTERM 
{
  printf("a dummy signal handler1\n");
  //exit(0); 

}  
void sig_hdl2(int signo) //this handler is used to setup custom 
                         //action for SIGINT 
{
  printf("a dummy signal handler2\n");
  kill(getpid(),SIGKILL); 

}

int main(int argc, char *argv[]) 
 { 

     int ret;
     sigset_t set1,set2;

     

     if (argc != 2) 
	{printf("Please enter two commands\n");
         exit(2);} 
   
      sigfillset(&set1);  //every bit in this user-space bit map is set !!!
      sigdelset(&set1, SIGTERM);  //specific bit of SIGTERM in the 
                                  //user-space bit map is reset - 
                                  //meaning, set to 0 


     sigprocmask(SIG_SETMASK, &set1,&set2) ;
   

     printf("we are blocked in the first sigsuspend\n"); 

     //sigsuspend(&set1); //block until an unmasked signal arrives  

     size_t mb = strtoul(argv[1], NULL, 0); 
 
     // Allocate the memory 
     //size_t nbytes = mb * 0x100000;
     unsigned long long int nbytes = mb * 0x100000;
 //    printf("size of nbytes is %lu\n", sizeof(nbytes)); 
   
     char bufs[1024];


     sprintf(bufs,"%llu\n", nbytes);

     printf("the actual value is %s\n",bufs); 
 
    

     char *ptr = (char *) malloc(nbytes); 
     if (ptr == NULL) { 
         perror("malloc1"); 
         exit(EXIT_FAILURE); 
     } 


     act1.sa_handler = sig_hdl1;  //ptr to our signal handler, in user space !!!
    

  
    sigfillset(&act1.sa_mask);

     act1.sa_flags = 0;  //currently, we ignore the flags
  

     sigaction(SIGTERM,&act1,&act2); 

     act1.sa_handler = sig_hdl2;
     act1.sa_flags = 0;  //currently, we ignore the flags
     sigfillset(&act1.sa_mask);
     sigaction(SIGINT,&act1,&act2);

     sigfillset(&set1);
     sigdelset(&set1, SIGTERM); 
     //sigfillset(&set1);
     sigdelset(&set1, SIGINT); 
     ret = sigtimedwait(&set1,NULL,NULL);//this process normally blocks here 

     printf("the starting address of the mem. is %x\n", (unsigned int)ptr); 
     // Touch the memory (could also use calloc()) 
     size_t i; 
     const size_t stride = sysconf(_SC_PAGE_SIZE); 
     for (i = 0; i < nbytes; i += stride) { 
       ptr[i] = 0; 
     } 

     sigfillset(&set1);
     sigprocmask(SIG_SETMASK, &set1,NULL) ; 
 
     printf("allocated %d mb\n", mb); 
     
     //page frame allocations by this process !!!
     while(1){
     sigdelset(&set1, SIGTERM);
     sigdelset(&set1, SIGINT);
     ret = sigtimedwait(&set1,NULL,NULL);
     }  
     


     return 0; 










}
