#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<string.h>
#define FIFO "/home/ashish/C_DAC/OS/assignment4/myfifo"
int main(int argc,char *argv[])
{
	int ret,ret1,mode,i;
	char ch[20];
	if(argc<2)
	{
	fprintf(stderr,"use: %s should be some combination of O_RDONLY O_WRONLY O_NONBLOCK\n",*argv);
	exit(EXIT_FAILURE);
	}
	for(i=1;i<argc;i++){
	if(strncmp(*++argv,"O_RDONLY",8)==0)
	mode|=O_RDONLY;
	 if(strncmp(*argv,"O_WRONLY",8)==0)
        mode|=O_WRONLY;
	 if(strncmp(*argv,"O_NONBLOCK",10)==0)
        mode|=O_NONBLOCK;
	}
	if(access(FIFO,F_OK)== -1)
	{
		ret=mkfifo(FIFO,0777);
		{ if(ret!=0)fprintf(stderr,"could not create %s the fifo\n",FIFO);
		exit(0);}
	}
	printf("process %d opening pipe\n",getpid());
	ret1=open(FIFO,mode);
	printf("Process %d raed %d form pipe\n",getpid(),ret1);
	sleep(5);
	if(ret1!=-1)
	{
	(void)close(ret1);
	printf("process %d finishes\n",getpid());}
	exit(0);	

}
	
