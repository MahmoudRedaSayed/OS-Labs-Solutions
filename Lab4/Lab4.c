#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<sys/wait.h> 
/*
    The solution of The ATs problem
        The main function will take parameters in running time :
            1-File path
            2-N(The number of ATs)
            3-P(The min grade)
        S: The number of the students 
        Stat_loc: Variable used in the exit of the process
        count: i will use it to know the position of the next process 
                in the file to read and to avoid if the S is not divisable by N
        Prop : File Pointer use it to open the file
        ATS : Array of ATs use it to save the Ids of the processes in ordered way 
        Successfull : Array to save the number of students succeed in the every AT at the end and use it to print
        Mid : variable to read Mid grade 
        Final : variable to read Final grade
        
*/ 
int main(int argc,char* argv[])
{
    // -------------------Variables-----------------//
    FILE *Prop=fopen(argv[1],"r");
    int N=atoi(argv[2]) ;
    int P=atoi(argv[3]) ;
    int count=0 ;
    int Stat_loc ; 
    int S ;
    int Mid;
    int Final;
    int* ATS=malloc(sizeof(int)*N);
    int* Successfull=malloc(sizeof(int)*N);
    //-------------------------------------------------//

    // Reading The number of the students 
    fscanf(Prop,"%2d",&S);

    
    // Loop to create processes to act as ATs
    for(int i=0;i<N;i++) 
    {   
        // This check to avoid if the S is not divisable by N
        if(i!=N-1)
        {
            count+=(S/N);
            //To save the Id of the process
            ATS[i]=fork();

            if(ATS[i]== 0)
            {
                int CountSuccess=0;
                for(int k=0;k<count-(S/N);k++)
                    {
                        //overriding To reach the correct position
                        fscanf(Prop,"%2d",&Mid);
                        fscanf(Prop,"%2d",&Mid);
                    }

                for(int j=0;j<(S/N);j++)
                {
                    // The required position to read
                    fscanf(Prop,"%2d",&Mid);
                    fscanf(Prop,"%2d",&Final);
                    if(Mid+Final>=P)
                    {
                        CountSuccess++;
                    }
                }
                // exit the number of Succeed Students
                exit(CountSuccess);
            }
        }
        else
        {
            ATS[i]=fork();
            if(ATS[i] == 0)
            {
                int Mid;
                int Final;
                int CountSuccess=0;
                for(int k=0;k<count;k++)
                    {
                        //overriding To reach the correct position
                        fscanf(Prop,"%2d",&Mid);
                        fscanf(Prop,"%2d",&Mid);
                    }

                for(int j=0;j<(S-count);j++)
                {
                    // The required position to read
                    fscanf(Prop,"%2d",&Mid);
                    fscanf(Prop,"%2d",&Final);
                    if(Mid+Final>=P)
                    {
                        CountSuccess++;
                    }
                }
                // exit the number of Succeed Students
                exit(CountSuccess);
            }
        }

    }

    //----------------------------------------------------------//
    // Loop to wait the processes 
    for(int i=0;i<N;i++) 
    {
        int Pid=wait(&Stat_loc);
        //loop to save the number of succeed in the correct position 
        for(int j=0;j<N;j++ )
        {
                if(Pid==ATS[j])
                {
                    // take the number of succeed from every ATs
                        Successfull[j]=WEXITSTATUS(Stat_loc);
                }
        }
    }
    //--------------------------------------------------------------//

    //--------------------------------------------------------------//
    //loop to print the data
    for(int i=0;i<N;i++)
    {
            printf("%d ",Successfull[i]);
    }
    printf("\n");
}
      