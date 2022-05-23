#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define maxSize 128 //  each char stored in 2 bytes

int shm_id,sem1,sem2;
void *shmaddr;
/* arg for semctl system calls. */
union Semun
{
    int val;               /* value for SETVAL */
    struct semid_ds *buf;  /* buffer for IPC_STAT & IPC_SET */
    ushort *array;         /* array for GETALL & SETALL */
    struct seminfo *__buf; /* buffer for IPC_INFO */
    void *__pad;
};
void down(int sem)
{
    struct sembuf p_op;

    p_op.sem_num = 0;
    p_op.sem_op = -1;
    p_op.sem_flg = !IPC_NOWAIT;

    if (semop(sem, &p_op, 1) == -1)
    {
        perror("Error in down()");
        exit(-1);
    }
}

void up(int sem)
{
    struct sembuf v_op;

    v_op.sem_num = 0;
    v_op.sem_op = 1;
    v_op.sem_flg = !IPC_NOWAIT;

    if (semop(sem, &v_op, 1) == -1)
    {
        perror("Error in up()");
        exit(-1);
    }
}
void writeInSharedMemory(int shmid, char *word)
{

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
        down(sem2);
        printf("\nReversed : %s\n", (char *)shmaddr);
    }
}

int main()
{
    int p_id;
    key_t key1 = ftok("keyFile", '1'); 
    key_t key2 = ftok("keyFile", '2'); 
    key_t key3 = ftok("keyFile", '3'); 
    union Semun semun;
    shm_id = shmget(key1, 4096, IPC_CREAT | 0666);

    if (shm_id == -1)
        perror("Error in creating shared memory\n");
    else
        printf("Shared Memory Id : %d\n", shm_id);
    shmaddr = shmat(shm_id, (void *)0, 0); // attach

    sem1 = semget(key2, 1, 0666 | IPC_CREAT);
    if (sem1 == -1)
    {
        perror("Error in create sem");
        exit(-1);
    }
    else
        printf("\nSemaphore 1 ID = %d\n", sem1);
    sem2 = semget(key3, 1, 0666 | IPC_CREAT);
    if (sem2 == -1)
    {
        perror("Error in create sem");
        exit(-1);
    }
    else
        printf("\nSemaphore 2 ID = %d\n", sem2);
    semun.val = 0; /* initial value of the semaphore, Binary semaphore */
    if (semctl(sem1, 0, SETVAL, semun) == -1)
    {
        perror("Error in semctl");
        exit(-1);
    }
    if (semctl(sem2, 0, SETVAL, semun) == -1)
    {
        perror("Error in semctl");
        exit(-1);
    }

    fflush(stdout);
    // get the input
    char *input = calloc(maxSize, sizeof(char));
    while (1)
    {
        fgets(input, maxSize, stdin);
        up(sem1);
        writeInSharedMemory(shm_id, input);
    }
    return 0;
}