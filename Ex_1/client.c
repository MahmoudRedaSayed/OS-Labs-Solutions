 #include <sys/types.h>
#include <sys/ipc.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/msg.h>


struct msgBuffer
{
    long msgType;
    char message[80];
};




int main()
{
    // Get the process id and get the last 4 digits from it
    int ProcessId = getpid();
    int LastFourDigits = ProcessId % 10000;
    
    char* word = calloc(80, sizeof(char));

    key_t Id;
    int RecMsgQueueID, SendMsgQueueID;

    Id = ftok("keyfile", 66);

    // try to allocate buffer in the memory
    //RecMsgQueueID   = msgget(keyId, 0666 | IPC_CREAT);
    //SendMsgQueueID = msgget(keyId, 0666 | IPC_CREAT);

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
    printf("My P_id = %d\n", ProcessId);
    
    struct msgBuffer MsgBuffer;
    while (1)
    {
        // get the message from the client
        fgets(word, 80, stdin);


        // set type of the message with the last four digits and copy the word into the message
        MsgBuffer.msgType = LastFourDigits;
        strcpy(MsgBuffer.message, word);

        // send the word to the server be reversed
        int SendFlag = msgsnd(RecMsgQueueID, &MsgBuffer, sizeof(MsgBuffer.message),! IPC_NOWAIT);

        // check if failed to send ?
        if (SendFlag == -1)
            perror("Error in sending");

        // recive the reversed word
        int RecvFlag = msgrcv(SendMsgQueueID, &MsgBuffer, sizeof(MsgBuffer.message), MsgBuffer.msgType, !IPC_NOWAIT);

        // check if failed to recive ?
        if (RecvFlag == -1)
            perror("Error in receiving");
        else
            printf("\nreversed word : %s\n", MsgBuffer.message);
    }
    
return 0;
}
