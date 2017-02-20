#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "CoreDataAndFunctions.h"

void HandleTCPClient(int clntSocket);

char buffer[SOCK_BUFFSIZE];

char userName[12];
char password[12];

bool wannaExit(char* in)
{
    if (!in)
    {
        return false;
    }
    size_t i = 0;
    
    while(1)
    {
        if(in[i] == '\0' || in[i+1] =='\0'|| in[i+2] == '\0' )
        {
            return false;
        }
        if(in[i]=='b' && in[i+1] == 'y' && in[i+2] == 'e')
        {
           if(in[i+3]=='\0' || in[i+3] == ' ')
           {
               return true;
           }
        }
        i+=1;
    }
    return false;
}


void printData(char * buffer)
{
    for(int i = 0; i < SOCK_BUFFSIZE; i++ )
    {
        char a = buffer[i];
        if(a == ' ')
        {
            putchar('\n');
        }
        else if(a == '\0')
        {
            break;
        }
        putchar(a);
    }
    printf("\n--------------------------------\n");
}


void getData(int servSocket, const char* cmd)
{
    int bytesRcvd = 0;
    int totalBytesRcvd = 0;
    send(servSocket, cmd, 1, 0);
    
    
    if ((bytesRcvd = recv(servSocket, buffer, SOCK_BUFFSIZE, 0)) <= 0)
        perror("recv() failed or connection closed prematurely");
    totalBytesRcvd += bytesRcvd;
    
    printf(buffer);
}


size_t getInput(char *question, char *inputBuffer, int bufferLength, size_t start_idx)
{
    size_t totalChar = start_idx;
    printf("%s ", question);
    
    char in = fgetc(stdin);
    
    while (in == '\n' || in == ' ')
    {
        in = fgetc(stdin);
    }
    
    while(in != '\n' && totalChar < bufferLength-1 )
    {
        inputBuffer[totalChar] = in;
        in = fgetc(stdin);
        totalChar += 1;
    }
    
    inputBuffer[totalChar] = '\0';
    
    return totalChar += 1;
}

void sendMessage(int servSocket)
{
    buffer[0] = '2';
    char recv = 0;
    scanf("%c", &recv);
    printf("Enter reciver's userid:");
    scanf("%c", &recv);
    buffer[1]=recv;
    buffer[2] = '<';
    
    size_t input_size = getInput("\nEnter Message", buffer, SOCK_BUFFSIZE, 3);
    strcat(buffer, ">");
    
    if(send(servSocket, buffer, sizeof(buffer), 0)<0)
        perror("Unable to send message");
    else
        printf("\nMessage sent successfully\n----------------------");
}

void getMessages(int servSocket)
{
    //TODO: Maintain who the current user is in the server.
    //I am already doing it.
    
    
}

void getLine(char * buffer)
{
    getchar();
    scanf ("%[^\n]%*c", buffer);
}

//Prints data sepearated by space on new lines.



void getUserList(int servSocket)
{
    
    int bytesRcvd = 0;
    int totalBytesRcvd = 0;
    send(servSocket, "1",1, 0);
    printf("Receiving user List................\n ");
    //For now lets assume all the data is received in one shot.
    if ((bytesRcvd = recv(servSocket, buffer, SOCK_BUFFSIZE, 0)) <= 0)
        DieWithError("recv() failed or connection closed prematurely");
    totalBytesRcvd += bytesRcvd; /* Keep tally of total bytes */
    buffer[bytesRcvd] = '\0'; /* Terminate the string! */
    
    printData(buffer);
    
    
}

int createSock(char *servlP, unsigned short port)
{
    int sock = -1;
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        perror("Failed to create socket. Please try again.");
    
    struct sockaddr_in servAddr;
    
    
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET; /* Internet address family */
    servAddr.sin_addr.s_addr = inet_addr(servlP); /* Server IP address */
    servAddr.sin_port = htons(port); /* Server port */
    
    if (connect(sock, (struct sockaddr *) &servAddr, sizeof(servAddr)) < 0)
        perror("connect() failed. Try again.");
    
    return sock;
}

int loginToServer(char *servlP, unsigned short port)
{
    
    
    buffer[0]='0';
    buffer[1]='-';
    strcat(buffer, userName);
    strcat(buffer, "%");
    strcat(buffer, password);
    strcat(buffer, "%");
    
    int sock = createSock(servlP, port);
    if (send(sock, buffer, SOCK_BUFFSIZE, 0) < SOCK_BUFFSIZE)
    {
        perror("Failed to login. Please try again");
        
    };
    if (recv(sock, buffer, SOCK_BUFFSIZE, 0) < SOCK_BUFFSIZE)
    {
        if(buffer[0]!='0')
        {
                perror("Failed to recv login confirmation. Try Again");
                return -1;
        }
        else
        {
            printf("Successfully connected to the server");
            return sock;
        }
        
        
        
    };
    
    return sock;
}

//TODO: bye to disconnect..
//Client reads it and sends appropriate msg to the server.
void chatMsg (char * buffer, char * msg)
{
    strcpy(msg, userName);
    strcat(msg, ": ");
    strcat(msg, buffer);
}

void chatHandler(unsigned short sock)
{
    int recvMsgSize = 0;
    while (1) {
        while ((recvMsgSize = recv(sock, buffer, SOCK_BUFFSIZE, 0)) < SOCK_BUFFSIZE)
        {
            if(recvMsgSize == 0)
            {
                printf("\nChat Ended.");
                break;
            }
            
        }
        if(recvMsgSize == 0)
        {
            break;
        }
        printf(buffer);
        puts("\n");
        getInput("You:", buffer, SOCK_BUFFSIZE,0);
        char msg[SOCK_BUFFSIZE];
        chatMsg(buffer, msg);
        if(wannaExit(buffer))
        {
            printf("\nEnding chat");
            close(sock);
            
            return;
        }
    
    if (send(sock,msg, SOCK_BUFFSIZE, 0) < 0)
            perror("send() failed");

    }
    printf("\n Waiting for other chatters.");
}

//TODO: Maintain username in the client
void joinChat()
{//get ip
    char inputBuff[100];
    int port = 0;
    printf("\n Enter port number:");
    scanf("%d", &port);
    int sock = createSock("127.0.0.1", port);
    if(sock<=0)
    {
        printf("Failed to join chat");
        return;
    }
    int recvMsgSize = 0;
    while(1){
        getInput("You:", buffer, SOCK_BUFFSIZE,0);
        if (wannaExit(buffer)) {
            close(sock);
            return;
        }
        char msg[200];
        chatMsg(buffer, msg);
        if (send(sock, msg, SOCK_BUFFSIZE, 0) < SOCK_BUFFSIZE)
            perror("send() failed");
        
        while ((recvMsgSize = recv(sock, buffer, SOCK_BUFFSIZE, 0)) < SOCK_BUFFSIZE)
        {
            if(recvMsgSize == 0)
            {
                printf("\nChat Ended.");
                return;
            }
            
        }
        printf(buffer);
        printf("\n");
    }
}

void initializeChat()
{
    int port = 0;
    printf("\n Enter Port:");
    scanf("%d", &port);
    printf("\n Intializing Chat");
    setupAndListenSocket(port, &chatHandler);
}





int main(int argc, char *argv[])
{
    bool stillRunning = true;
    char servIP[100];
    int servPort;
    int servSocket = 0;
    char messageBuffer[100];
    
    while(stillRunning)
    {
        printf("0 Login To the Server\n");
        printf("1 Get the User List\n");
        printf("2 Send a message\n");
        printf("3 Get my messages\n");
        printf("4 Start a chat\n");
        printf("5 join a chat\n");
        printf("6 Exit.\n");
        int choice;
        fflush(stdout);
        scanf("%d", &choice);
        switch (choice) {
            case 6:
                stillRunning = false;
                break;
            case 0:
                fflush(stdout);
                printf("IP address: ");
                fflush(stdout);
                getchar();
                scanf ("%[^\n]%*c", &servIP);
                printf("\nPort: ");
                fflush(stdout);
                scanf("%d", &servPort);
                getInput("\nUserName:", userName, 12, 0);
                getInput("\nPassWord", password, 12,0);
                servSocket = loginToServer(servIP, servPort);
                //servSocket = loginToServer("127.0.0.1", servPort);
                if (servSocket > 0)
                {
                    printf("Connected to Server on port: %d \n\n\t\t ---------", servSocket);
                }
                else
                {
                    printf("failed to connect to Server.");
                    
                }
                printf("\n");
                break;
            case 1:
                //if(servSocket == 0) printf("Connect to server first");
                //continue;
                getUserList(servSocket);
                break;
            case 2:
                //if(servSocket == 0) printf("Connect to server first");
                sendMessage(servSocket);
                continue;
                break;
            case 3:
                getData(servSocket, "3");
                break;
            case 4:
                //if(servSocket == 0) printf("Connect to server first");
                //continue;
                initializeChat();
                break;
            case 5:
                //if(servSocket == 0) printf("Connect to server first");    
                joinChat();
                continue;
                break;
            default:
                fprintf(stderr, "Invalid Choice. Please chose from available options.\n");
                break;
        }
        
    }
    
    
    //char echoBuffer[RCVBUFSIZE]; /* Buffer for echo string */
    //unsigned int echoStringLen; /* Length of string to echo */
    //int bytesRcvd, totalBytesRcvd; /* Bytes read in single recv() and total bytes read */





    //echoStringLen = (unsigned int)strlen(echoString); /* Determine input length */


/* Send the string to the server */
    //if (send(sock, echoString, echoStringLen, 0) != echoStringLen)
        DieWithError("send() sent a different number of bytes than expected");


/* Receive the same string back from the server */
//    totalBytesRcvd = 0;
//    printf("Received: "); /* Setup to print the echoed string */
//    while (totalBytesRcvd < echoStringLen)
//    {
//        
//        /* Receive up to the buffer size (minus i to leave space for a null terminator) bytes from the sender */
//        if ((bytesRcvd = recv(sock, echoBuffer, RCVBUFSIZE - 1, 0)) <= 0)
//            DieWithError("recv() failed or connection closed prematurely");
//        totalBytesRcvd += bytesRcvd; /* Keep tally of total bytes */
//        echoBuffer[bytesRcvd] = '\0'; /* Terminate the string! */
//        printf(echoBuffer); /* Print the echo buffer */
//    }
//
//    printf("\n"); /* Print a final linefeed */

    //TODO: need to close the socket
    //close(sock);
    exit(0);
}
