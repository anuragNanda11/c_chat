#include <stdlib.h>
#include <string.h>
#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for recv() and send() */
#include <unistd.h> /* for close() */

#include "CoreDataAndFunctions.h"

userAccount g_accounts[NUM_USERS];
size_t g_currUsrId;

void DieWithError(char *errorMessage); /* Error handling function */

void sendUserList(int clntSocket)
{
    char userList[100];
    
    strcpy(userList, g_accounts[0].username);
    strcat(userList, " ");
    
    for (size_t i  = 1;  i < NUM_USERS; i++) {
        strcat(userList, g_accounts[i].username);
        strcat(userList, " ");
    }
    
    if (send(clntSocket, userList, sizeof(userList), 0) < 0)
    {
        DieWithError("sendUserList() failed");
    }
    
}

void recvUserMessage(size_t userid, char buffer[])
{
    //add curr user name to the messag:
    g_accounts[userid].num_msgs +=1;
    size_t j = g_accounts[userid].idx_freespc;
    char * inboxRecv = g_accounts[userid].messages;
    
    char * sender = g_accounts[g_currUsrId].username;
    size_t s = 0;
    while(sender[s] != '\0')
    {
        inboxRecv[j] = sender[s];
        j++; s++;
    }
    inboxRecv[j]=':';
    inboxRecv[j+1] = ' ';
    j+=2;
    
    size_t i = 0;
    //get to the beg of the message
    while (buffer[i] != '<') {
        i++;
    }
    i++;
    
    while(buffer[i] != '>')
    {
        inboxRecv[j] = buffer[i];
        j++; i++;
    }
    
    g_accounts[userid].messages[j] = '\n';
    inboxRecv[j+1] = '\0';
    g_accounts[userid].idx_freespc = j+1;
    
}


void retriveUserMsgs(int clntSocket)
{
    char *msgs = g_accounts[g_currUsrId].messages;
    if(send(clntSocket,msgs,150,0)<0)
        perror("Unable to send messages to user");
    else
        printf("\nMessages sent successfull to user");
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
    
    while(write < 12)
    {
        read++;
        c = info[read];
        
        if(c=='%') break;
        
        nm[write] = c;
        write++;
    }
    nm[write] = '\0';
    
    write = 0;
    while(write < 12)
    {
        read++;
        c = info[read];
        if (c=='%') break;
        pswd[write] = c;
        write++;
    }
    pswd[write] = '\0';
    
    bool userVerified = false;
    size_t userid;
    for (userid= 0; userid < NUM_USERS; ++userid) {
        if (strcmp(g_accounts[userid].username, nm) == 0) {
            if(strcmp(g_accounts[userid].password, pswd) == 0)
                userVerified = true;
            
            break;
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
    char buffer[SOCK_BUFFSIZE]; /* Buffer for echo string */
    int recvMsgSize = 0; /* Size of received message */
    //if ((recvMsgSize = recv(clntSocket, buffer, RCVBUFSIZE, 0)) < 0)
            //DieWithError("recv() failed") ;
    //Send User List:
    if ((recvMsgSize = recv(clntSocket, buffer, SOCK_BUFFSIZE, 0)) < 0)
        DieWithError("recv() failed") ;

        while (recvMsgSize > 0) /* zero indicates end of transmission */
        {
            //User Login;
            if (buffer[0]=='0')
            {
                
                if(userLogin(buffer)==true)
                {
                    send(clntSocket, "0", 1, 0);
                }else
                {
                    send(clntSocket, "9", 1, 0);
                }
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
            if(buffer[0] == '3')
            {
                retriveUserMsgs(clntSocket);
            }
            if ((recvMsgSize = recv(clntSocket, buffer, SOCK_BUFFSIZE, 0)) < 0)
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
