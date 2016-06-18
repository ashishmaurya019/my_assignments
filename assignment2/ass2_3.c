#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<sys/types.h>
int main()
{
	int ret4,ret1,ret0,ret2,ret3;
		int a=2,b=3,sum=0;
			int ret,status=0;
				unsigned long int i=0;
					printf("parent...ppid is %d....and pid is %d\n",getppid(),getpid());
						printf("sum is %d\n\n",sum=a+b);
							while(i++<6)
									{
												ret=fork();

														if(ret<0)
																	{
																					perror("error in creation");
																								exit(3);
																										}
																if(ret>0)
																			{
																						/*	printf("I am parent...ppid is %d...pid is %d...ret is %d\n",getppid(),getpid(),ret);
																						 *			*/	continue;
																						}
																		if(ret==0)
																					{
																								if(i==1){
																												//printf("I am child...ppid is %d...pid is %d\n",getppid(),getpid());
																												//			ret=execl("/usr/bin/gcc","gcc","sum.c","-c","-o","sum.o",NULL);
																												//					
																												//								if(ret<0){perror("error in loading");exit(0);}
																												//											exit(0);}
																												//													 if(i==2){
																												//													 			ret=execl("/usr/bin/gcc","gcc","sub.c","-c","-o","sub.o",NULL);
																												//													 					
																												//													 								if(ret<0){perror("error in loading");exit(0);}
																												//													 											exit(0);}
																												//													 													if(i==3){
																												//													 														                 ret=execl("/usr/bin/gcc","gcc","div.c","-c","-o","div.o",NULL);
																												//													 														                 		
																												//													 														                 					 if(ret<0){perror("error in loading");exit(0);}
																												//													 														                 					 				 exit(0);}
																												//													 														                 					 				 		 if(i==4){
																												//													 														                 					 				 		 	                         ret=execl("/usr/bin/gcc","gcc","mul.c","-c","-o","mul.o",NULL);
																												//													 														                 					 				 		 	                         				 if(ret<0){perror("error in loading");exit(0);}
																												//													 														                 					 				 		 	                         				 				 exit(0);}
																												//													 														                 					 				 		 	                         				 				 		if(i==5)
																												//													 														                 					 				 		 	                         				 				 					{
																												//													 														                 					 				 		 	                         				 				 							//	if(ret0>0 && ret1>0 && ret2>0 && ret3>0)
																												//													 														                 					 				 		 	                         				 				 										ret=execl("/usr/bin/gcc","gcc","main.c","-c","-o","main.o",NULL);
																												//													 														                 					 				 		 	                         				 				 													 if(ret<0){perror("error in loading");exit(0);}
																												//													 														                 					 				 		 	                         				 				 													 						 exit(0);}
																												//													 														                 					 				 		 	                         				 				 													 						 		if(i==6)
																												//													 														                 					 				 		 	                         				 				 													 						 				{
																												//													 														                 					 				 		 	                         				 				 													 						 						ret=execl("/usr/bin/gcc","gcc","main.o","sum.o","sub.o","mul.o","div.o","-o","ak",NULL);
																												//													 														                 					 				 		 	                         				 				 													 						 								if(ret<0){perror("error in loading");exit(0);}
																												//													 														                 					 				 		 	                         				 				 													 						 								                exit(0);
																												//													 														                 					 				 		 	                         				 				 													 						 								                		}		
																												//													 														                 					 				 		 	                         				 				 													 						 								                					//	ret=execl
																												//													 														                 					 				 		 	                         				 				 													 						 								                							
																												//													 														                 					 				 		 	                         				 				 													 						 								                									}// end of child
																												//													 														                 					 				 		 	                         				 				 													 						 								                											
																												//													 														                 					 				 		 	                         				 				 													 						 								                												}//end of while1
																												//													 														                 					 				 		 	                         				 				 													 						 								                													if(ret>0)
																												//													 														                 					 				 		 	                         				 				 													 						 								                														{
																												//													 														                 					 				 		 	                         				 				 													 						 								                																while(1)
																												//													 														                 					 				 		 	                         				 				 													 						 								                																		{
																												//													 														                 					 				 		 	                         				 				 													 						 								                																				ret = waitpid(-1,&status,0);
																												//													 														                 					 				 		 	                         				 				 													 						 								                																						if(ret>0)
																												//													 														                 					 				 		 	                         				 				 													 						 								                																								{
																												//													 														                 					 				 		 	                         				 				 													 						 								                																											if(WIFEXITED(status))
																												//													 														                 					 				 		 	                         				 				 													 						 								                																														{
																												//													 														                 					 				 		 	                         				 				 													 						 								                																																		if(WEXITSTATUS(status)==0)
																												//													 														                 					 				 		 	                         				 				 													 						 								                																																						printf("Terminated Normally and succesfully\n");
																												//													 														                 					 				 		 	                         				 				 													 						 								                																																										else
																												//													 														                 					 				 		 	                         				 				 													 						 								                																																															printf("Terminated Normally but not succesfully\n");
																												//													 														                 					 				 		 	                         				 				 													 						 								                																																																		}
																												//													 														                 					 				 		 	                         				 				 													 						 								                																																																					else
																												//													 														                 					 				 		 	                         				 				 													 						 								                																																																									printf("Terminated Abnormally\n");
																												//													 														                 					 				 		 	                         				 				 													 						 								                																																																											}
																												//													 														                 					 				 		 	                         				 				 													 						 								                																																																													else
																												//													 														                 					 				 		 	                         				 				 													 						 								                																																																															{
																												//													 														                 					 				 		 	                         				 				 													 						 								                																																																																		printf("All child are terminated allready...No need to wait\n");
																												//													 														                 					 				 		 	                         				 				 													 						 								                																																																																					exit(0);
																												//													 														                 					 				 		 	                         				 				 													 						 								                																																																																							}
																												//													 														                 					 				 		 	                         				 				 													 						 								                																																																																								
																												//													 														                 					 				 		 	                         				 				 													 						 								                																																																																									      }
																												//													 														                 					 				 		 	                         				 				 													 						 								                																																																																									      	}//end of if
																												//													 														                 					 				 		 	                         				 				 													 						 								                																																																																									      		
																												//													 														                 					 				 		 	                         				 				 													 						 								                																																																																									      		return 0;
																												//													 														                 					 				 		 	                         				 				 													 						 								                																																																																									      		}:wq
																												//
