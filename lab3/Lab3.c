#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
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
        joy=argv[2];
        Kon=argv[1];
        for(int i=0;i<strlen(argv[2]);i++)
        {
            joy[i]=tolower(argv[2][i]);
        }
        for(int i=0;i<strlen(argv[1]);i++)
        {
            Kon[i]=tolower(argv[1][i]);
        }
    }
    else
    {
        joy=argv[1];
        Kon=argv[2];
        for(int i=0;i<strlen(argv[1]);i++)
        {
            printf("herre1 %c",tolower(argv[1][i]));
            joy[i]=tolower(argv[1][i]);
        }
        for(int i=0;i<strlen(argv[2]);i++)
        {
            printf("herre");
            Kon[i]=tolower(argv[2][i]);
        }
    }



    int* arr=malloc(sizeof(int)*strlen(joy));
    for(int i=0;i<strlen(joy);i++)
    {
        printf("from the loop \n");
        arr[i]=-1;
    }
    int counter=0;
    for(int i =0 ;i<strlen(Kon);i++)
    {
        if(Kon[i]==joy[counter])
        {
            printf("from the loop \n");
            arr[counter]=i;
            counter++;
        }
    }

    for(int i=0;i<strlen(joy)-1;i++)
    {
        printf("from the loop \n");
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