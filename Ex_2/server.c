#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>
#define maxSize 128 //  each char stored in 2 bytes

int shm_id;
/* reverse a message */
void reverse(char *msg, char *reversed)
{
    int i;
    for (i = 0; i < strlen(msg); ++i)
        reversed[i] = msg[strlen(msg) - i - 1];
}

void read_reverse(int shmid)
{
    void *shmaddr = shmat(shmid, (void *)0, 0);


    char *temp = calloc(maxSize, sizeof(char));
    char *reversedWord = calloc(maxSize, sizeof(char));

    reverse((char *)shmaddr, temp); // reverse the word
    reversedWord[0] = '#';
    strcat(reversedWord, temp);
    strcpy((char *)shmaddr, reversedWord); // put the reversed word in the shared memory
     sleep(1);

    shmdt(shmaddr);
}

void freeSources(int signum)
{
    shmctl(shm_id, IPC_RMID, (struct shmid_ds *)0);
    exit(-1);
}

int main()
{
    int p_id;

    signal(SIGINT, freeSources);

    int key = 0;
    // key_t shmKey = ftok("keyFile", 65);
    shm_id = shmget('x', 4096, 0666 | IPC_CREAT);

    if (shm_id == -1)
        perror("Error in creating shared memory\n");
    else
        printf("Shared Memory Id : %d\n", shm_id);

    while (1)
    {
        read_reverse(shm_id);
    }

    return 0;
}

