#include <stdlib.h>
#include <string.h>
#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h> /* for close() */

#include "CoreDataAndFunctions.h"
#define RCVBUFSIZE 32 /* Size of receive buffer */

userAccount g_accounts[NUM_USERS];
size_t g_currUsrId;

char userList[100] = "Nancy Minnie Bob Andrew";
void DieWithError(char *errorMessage); /* Error handling function */

void askForLogin(int clntSocket)
{
    
    send(clntSocket, "Logging in\n\n", 10,0);
    send(clntSocket, "Enter Username\n", 14,0);
    char buffer[RCVBUFSIZE];
    /* Receive message from client */
    int recvMsgSize; /* Size of received message */
    if ((recvMsgSize = recv(clntSocket, buffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed") ;
    
    close(clntSocket);
}

void sendUserList(int clntSocket)
{
    if (send(clntSocket, userList, sizeof(userList), 0) < 0)
    {
        DieWithError("sendUserList() failed");
    }
    
}

void recvUserMessage(size_t userid, char buffer[])
{
    buffer[0]=' ';buffer[1]=' ';buffer[2]=' '; buffer[3]=' ';
    strcat(g_accounts[userid].messages, buffer);
    g_accounts[userid].num_msgs +=1;
    //TODO: fix this.
    //g_accounts[userid].messages[msgSize] = ' ';
    //g_accounts[userid].idx_freespc+=msgSize + 1;
    
}


void retriveUserMsgs(int clntSocket)
{

}

bool userLogin(char info[])
{
   
    size_t read = 0;
    char c  = info[read];
    
    size_t write = 0;
    char nm [12];
    char pswd [12];
    
    
    //discard the commands
    while (c != '-')
    {
        read+=1;
        c = info[read];
    }
    read++;
    c = info[read];
    while((c != '\0' || c != '%') && write < 12)
    {
        nm[write] = c;
        read+=1;
        c = info[read];
    }
    nm[write] = '\0';
    
    write = 0;
    
    while(c!='\0' && write < 12)
    {
        read +=1;
        c = info[read];
        pswd[write] = c;
    }
    bool userVerified = false;
    size_t userid;
    for (userid= 0; userid < NUM_USERS; ++userid) {
        if (strcmp(g_accounts[userid].username, nm)) {
            if(strcmp(g_accounts[userid].password, pswd))
            {
                userVerified = true;
            }
        }
    }
    if (userVerified)
    {
        
        g_currUsrId = userid;
        return true;
    }
    g_currUsrId = -1;
    return false;
}


//TODO: reserve first few bytes for the commands. And rest for data. 
void HandleTCPClient(int clntSocket)
{
    char buffer[RCVBUFSIZE]; /* Buffer for echo string */
    int recvMsgSize = 0; /* Size of received message */
    //if ((recvMsgSize = recv(clntSocket, buffer, RCVBUFSIZE, 0)) < 0)
            //DieWithError("recv() failed") ;
    //Send User List:
    if ((recvMsgSize = recv(clntSocket, buffer, RCVBUFSIZE, 0)) < 0)
        DieWithError("recv() failed") ;

        while (recvMsgSize > 0) /* zero indicates end of transmission */
        {
            //User Login;
            if (buffer[0]=='0')
            {
                
                userLogin(buffer);
            }
            
            if (buffer[0] == '1')
            {
                sendUserList(clntSocket);
                //NUll that arg.
                buffer[0] = 'n';
            }
            if(buffer[0] == '2')
            {
                recvUserMessage(atoi(&buffer[1]), buffer);
            }
            if ((recvMsgSize = recv(clntSocket, buffer, RCVBUFSIZE, 0)) < 0)
                DieWithError("recv() failed") ;
    
        }
    //    if(strcmp(buffer[0], "1"))
//    {
//        sendUserList(clntSocket);
//    }
    

    

//    /* Send received string and receive again until end of transmission */
//    while (recvMsgSize > 0) /* zero indicates end of transmission */
//    {
//
//        /* Echo message back to client */
//        if (send(clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize)
//            DieWithError("send() failed");
//
//        /* See if there is more data to receive */
//        if ((recvMsgSize = recv(clntSocket, echoBuffer, RCVBUFSIZE, 0)) < 0)
//            DieWithError("recv() failed") ;
//
//
//    }
    

    //close(clntSocket); /* Close client socket */
}
