#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#define maxSize 128

void writeInSharedMemory(int shmid, char *word)
{
    void *shmaddr = shmat(shmid, (void *)0, 0); // attach

    if (shmaddr == -1)
    {
        perror("Error in attaching in Writer\n");
        exit(-1);
    }
    else
    {
        char *result = calloc(maxSize, sizeof(char));
        
        // write the word in shared memory
        strcpy((char *)shmaddr, word);

        strcpy(word, (char *)shmaddr);
        while (word[0] != '#')      // # : an indication to know if the server has finished the reversing
        {
            strcpy(word, (char *)shmaddr);
        }
        // remove char # from the start
        int len = strlen(word);
        for (int i = 0; i < len - 1; i++)
        {
            result[i] = word[i + 1];
        }

        strcpy((char *)shmaddr, "\0");
        printf("\nReversed : %s\n", result);
    }
}

int main()
{
    int p_id;

    int shm_id = shmget('x', 4096, IPC_CREAT | 0666);

    if (shm_id == -1)
        perror("Error in creating shared memory\n");
    else
        printf("Shared Memory Id : %d\n", shm_id);

    // get the input
    char *input = calloc(maxSize, sizeof(char));
    fgets(input, maxSize, stdin);

    while (1)
    {
        writeInSharedMemory(shm_id, input);
        fgets(input, maxSize, stdin);
    }
    return 0;
}