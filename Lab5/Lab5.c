// The written quesations
// 	1- The parent because i put the signal handler in the start of the main and the parent only who will see it
// 	2- The parent will receive The exit code if not from the child who get the element will be from the second child who will terminate normally with exit code 0 
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
// some global variables
/*
   Child : is used to carray the Pid of the children
   stat_loc : to be used in the main function and the handler function
*/
int stat_loc;
int Child[2]={-2,-2};
/*
   handler function to handle the signal on the parent process
   it is wait to the process to terminate(exit) and then use the child array to check if it is the first 
   child or the second child and then kill all processes by using the function killpg

*/
void handler(int signum){
  int pid = wait(&stat_loc);
	
          if(!(stat_loc & 0x00FF))
	  {
	  if(pid==Child[0])
	  {
              printf("\nA first Child send the position  %d\n", stat_loc>>8);
	  }
	  else if(pid == Child[1])
	  {

                 printf("\nA second Child send the position  %d\n", stat_loc>>8);
	  }
	  }
  killpg(getpgid(getpid()), SIGINT);

}

/*
   The main function:
      first will put the handler of the SIGUSR1 
      and the will take the parameters 
         value: the target value to search for
         Array: to carray the array of the element to search in it
         order: to set it in the loop to know the order of the process
      Then will print the intial message 
      then will fork two processes
      then will go into the two processes by using the array child
      The first process will search in the first half and second will search in the second half
      if the element is found the process will send the position in the exit() like exit code
      if not the process will sleep to 3 seconds and then terminate
      and the parent will wait in the end 
*/
void main(int argv,char* argn[])
{
    signal (SIGUSR1, handler);
//-------------------------------------//
    int Value = atoi(argn[1]);
    int * Array=malloc(sizeof(int)*(argv-2));
//-------------------------------------//
      for(int i=0;i<argv-2;i++)
      {
         Array[i]=atoi(argn[i+2]);
      }
      printf("I am the parent an my process id is %d \n",getpid());
      for(int i=0;i<2;i++)
      {
	      if(Child[0]!=0)
	      {
	      	Child[i]=fork();
	      }
      }
      if(Child[0]==-1||Child[1]==-1)
      {
	      printf("Error in creatation");
	      return;
      }
//-------------------------------------//
        if(Child[0]==0)
         {
			      printf("I am the first child and my id=%d and my parent id=%d \n",getpid(),getppid());
			      for(int j=0;j<(argv-2)/2;j++)
               {
                  if(Value==Array[j])
                  {
                     kill(getppid(),SIGUSR1);
                     exit(1+j);
                  }
               }
               sleep(3);
               printf("The first child is terminated\n");
         }
		else if(Child[1]==0)
         {
			      printf("I am the second child and my id=%d and my parent id=%d \n",getpid(),getppid());
               for(int j=(argv-2)/2;j<argv;j++)
               {
                        if(Value==Array[j])
                        {
                           kill(getppid(),SIGUSR1);
                           exit(1+j);
                        }       
               }
               sleep(3);
               printf("The second child is terminated\n");               
         }
//-------------------------------------//
	if(Child[0]!=0&&Child[1]!=0)
	{
		sleep(5);
     	 for(int i=0;i<2;i++)
     	 {
        	int pid = wait(&stat_loc);
     	 }

     	 printf("The value not Found\n");
	 printf("The parent is terminated\n");

	}

}