#include<stdio.h>
#include<stdlib.h>
#include<string.h>
/* The problem definiation and Solution's Algo :
    
    First  : i will check if the number of the args is enough or not 
    Second : i will check if the two elements are identical or not 
    Third  : i will compare to know the longest args to be the Kon and assign the values in the lower case 
    Fourth : i will create array to save the position of the chars of the joy in the kon and itiaily by -1 
                and then check if the array is sorted or not and if it is correct the array will be sorted 
                otherwise the answer will be 0
*/
int main(int argc,char* argv[]){

    if(argc<2)
    {
        printf("too few args...\n");
        exit(1);
    }

    if(strlen(argv[1])==strlen(argv[2]))
    {
        int counter=0;
        for(int i=0;i<strlen(argv[1]);i++)
        {
            if(argv[1][i]==argv[2][i])
            {
                counter++;
                continue;
            }
        }
        if(counter==strlen(argv[1]))
        {
            printf("0");
            return 0;
        }
    }


    char* Kon;
    char* joy;


    if(strlen(argv[1])>strlen(argv[2]))
    {
        joy=strlwr(argv[2]);
        Kon=strlwr(argv[1]);
    }
    else
    {
        joy=strlwr(argv[1]);
        Kon=strlwr(argv[2]);
    }



    int* arr=malloc(sizeof(int)*strlen(joy));
    for(int i=0;i<strlen(joy);i++)
    {
        arr[i]=-1;
    }
    int counter=0;
    for(int i =0 ;i<strlen(Kon);i++)
    {
        if(Kon[i]==joy[counter])
        {
            arr[counter]=i;
            counter++;
        }
    }

    for(int i=0;i<strlen(joy)-1;i++)
    {
        if(arr[i]<arr[i+1])
        {
            continue;
        }
        printf("0");
        return 0;
    }   
    printf("1"); 
    return 1;


}