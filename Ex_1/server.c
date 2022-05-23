#include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>
#include <signal.h>

    // will be global to use it in the signal
    int RecMsgQueueID, SendMsgQueueID;

/* reverse a message */
// this function i will use two msg queue to prevent using function strncpy

void ReverseWord(char* msg, char* reversed) {
int i;
for (i=0; i<strlen(msg); ++i)
    reversed[i] = msg[strlen(msg) - i - 1];

}

void DeletTheBuffer(char* buffer, int size)
{
    for (size_t i = 0; i < size; i++)
    {
        buffer[i] = '\0';
    }
    
}

struct msgBuffer
{
    long msgType;
    char message[80];
};

void removeSources(int signum);

int main()
{

    key_t keyId;
    
    signal(SIGINT, removeSources);
    keyId = ftok("keyfile.txt", 65);

     // try to allocate buffer in the memory
    //upMsgQueueID   = msgget(keyId, 0666 | IPC_CREAT);
    //downMsgQueueID = msgget(keyId, 0666 | IPC_CREAT);

    // or use the static key
    RecMsgQueueID   = msgget('m', 0666 | IPC_CREAT);
    SendMsgQueueID = msgget('m', 0666 | IPC_CREAT);


    if (SendMsgQueueID == -1)
    {
        perror("Error in creating send Queue");
        exit(-1);
    }

    if (RecMsgQueueID == -1)
    {
        perror("Error in creating rec Queue");
        exit(-1);
    }

    printf("Rec Message Queue ID = %d\n", RecMsgQueueID);
    printf("Send Message Queue ID = %d\n", SendMsgQueueID);

    struct msgBuffer MsgBuffer;
    struct msgBuffer SendMsgBuffer;

    while(1)
    {
        // recieve the word from the client
        int recvValue = msgrcv(RecMsgQueueID, &MsgBuffer, sizeof(MsgBuffer.message), 0, !IPC_NOWAIT);

        // check if failed to recive ?
        if (recvValue == -1)
            perror("Error in receive");
        else
            printf("\nMessage received : %s\n", MsgBuffer.message);

        // function to free the message from the old value
        DeletTheBuffer(SendMsgBuffer.message, 80);

        // reverse the word
        ReverseWord(MsgBuffer.message, SendMsgBuffer.message);

        // set the msg type to determine whom client to recieve
        SendMsgBuffer.msgType = MsgBuffer.msgType;
        
        // send the reversed word to the client
        int sendValue = msgsnd(SendMsgQueueID, &SendMsgBuffer, sizeof(SendMsgBuffer.message),! IPC_NOWAIT);

        // check if failed to send ?
        if (sendValue == -1)
            perror("Errror in sending the reversed word");
        else
            printf("Success to send\n");
    }
    
    // remove the msg queues
    msgctl(SendMsgQueueID, IPC_RMID, (struct msqid_ds *)0);
    msgctl(RecMsgQueueID, IPC_RMID, (struct msqid_ds *)0);

    return 0;
}


void removeSources(int signum)
{
    // remove the msg queues
     msgctl(SendMsgQueueID, IPC_RMID, (struct msqid_ds *)0);
    msgctl(RecMsgQueueID, IPC_RMID, (struct msqid_ds *)0);
	exit(0);
}
