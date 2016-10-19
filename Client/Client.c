#include <stdio.h> /* for printf() and fprintf() */
#include <sys/socket.h> /* for socket(), connect(), send(), and recv() */
#include <arpa/inet.h> /* for sockaddr_in and inet_addr() */
#include <stdlib.h> /* for atoi() */
#include <string.h> /* for memset() */
#include <unistd.h> /* for close() */

#define RCVBUFSIZE 32 /* Size of receive buffer */


void HandleTCPClient(int clntSocket);

#include "CoreDataAndFunctions.h"

#define BUFFERSIZE 100
char buffer[BUFFERSIZE + 1];

size_t getInput(char *question, char *inputBuffer, int bufferLength, size_t start_idx)
{
    //setup packet for sending to server.
    //i.e. add the command number to the begining so that the server knows
    //what to do with the message.
    size_t totalChar = start_idx;
    printf("%s  (Max %d characters)\n", question, bufferLength - 1);
    
    char in = fgetc(stdin);
    
    while (in == '\n' || in == ' ')
    {
        in = fgetc(stdin);
    }
    
    while(in != '\n' && totalChar < bufferLength-1 )
    {
//        if (in == '\n')
//        {
//            inputBuffer[totalChar] = '\0';
//        }
//        else if (totalChar == bufferLength-1)
//        {
//            
//        }
        inputBuffer[totalChar] = in;
        in = fgetc(stdin);
        totalChar += 1;
    }
    
    inputBuffer[totalChar] = '\0';
    
    return totalChar += 1;
    
    
//    
//    getchar();
//    fgets(inputBuffer, bufferLength, stdin);
//    if (inputBuffer[strlen(inputBuffer) -1] != '\n')
//    {
//        int dropped = 0;
//        while (fgetc(stdin) != '\n')
//            dropped++;
//        
//        if (dropped > 0) // if they input exactly (bufferLength - 1)
//            // characters, there's only the \n to chop off
//        {
//            printf("Woah there partner, your input was over the limit by %d characters, try again!\n", dropped );
//            getInput(question, inputBuffer, bufferLength);
//        }
//    }
//    else
//    {
//        inputBuffer[strlen(inputBuffer) -1] = '\0';
//    }
//    //TODO: fix this.
//    return 0;
}

void sendMessage(int servSocket)
{
    buffer[0] = '2';
    buffer[1] = ' ';
    
    size_t input_size = getInput("\nEnter Message: <User ID>  <Message><.>\n", buffer, BUFFERSIZE, 2);
    
    if(send(servSocket, buffer, input_size, 0)<0)
        perror("Unable to send message");
    else
        printf("\nMessage sent successfully\n----------------------");
}

void getMessages(int servSocket)
{
    //TODO: Maintain who the current user is. 
    buffer[0] = '3';
    buffer[1] = '\0';
}

void getLine(char * buffer)
{
    getchar();
    scanf ("%[^\n]%*c", buffer);
}


void getUserList(int servSocket)
{
    
    int bytesRcvd = 0;
    int totalBytesRcvd = 0;
    send(servSocket, "1",1, 0);
    printf("Receiving user List................\n ");
    //For now lets assume all the data is received in one shot.
    if ((bytesRcvd = recv(servSocket, buffer, 99, 0)) <= 0)
        DieWithError("recv() failed or connection closed prematurely");
        totalBytesRcvd += bytesRcvd; /* Keep tally of total bytes */
        buffer[bytesRcvd] = '\0'; /* Terminate the string! */
    
    for(int i = 0; i < BUFFERSIZE; i++ )
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

int loginToServer(char *servlP, unsigned short port)
{
//    char username [50];
//    char password [50];
//    printf("\nEnter username: ");
//    getLine(username);
//    printf("\nEnter password: ");
//    getLine(password);
//    char loginMessage[100];
//    strcpy(loginMessage,"0 ");
//    strcat(loginMessage, username);
//    strcat(loginMessage," ");
//    strcat(loginMessage, password);
    
    int sock; /* Socket descriptor */
    /* Create a reliable, stream socket using TCP */
    if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
        DieWithError(" socket () failed") ;
    
    struct sockaddr_in echoServAddr; /* Echo server address */
    /* Construct the server address structure */
    memset(&echoServAddr, 0, sizeof(echoServAddr)); /* Zero out structure */
    echoServAddr.sin_family = AF_INET; /* Internet address family */
    echoServAddr.sin_addr.s_addr = inet_addr(servlP); /* Server IP address */
    echoServAddr.sin_port = htons(port); /* Server port */
    /* Establish the connection to the server */
    if (connect(sock, (struct sockaddr *) &echoServAddr, sizeof(echoServAddr)) < 0)
        DieWithError(" connect () failed");
    
    //TODO: fix this hard codded stuff
    
    if (send(sock, "0-user_1%pswd1", 14, 0) < 14)
    {
        //to do check if the server returns success.
        perror("Failed to login. Please try again");
        //call login to server again.
    };
    
    return sock;
    //HandleTCPClient(sock);
    
    //close(sock);
}



int main(int argc, char *argv[])
{
    
//    if ((argc < 3) || (argc > 4)) /* Test for correct number of arguments */
//    {
//        fprintf(stderr, "Usage: %s <Server IP> <Echo Word> [<Echo Port>]\n", argv[0]);
//        exit(1);
//    }
//    unsigned short echoServPort; /* Echo server port */
//    char *servlP; /* Server IP address (dotted quad) */
//    //char *echoString; /* String to send to echo server */
//    
//    servlP = argv[1] ; /* First arg' server IP address (dotted quad) */
//    //echoString = argv[2] ; /* Second arg' string to echo */
//    
//    if (argc == 4)
//        echoServPort = atoi(argv[3]); /* Use given port, if any */
//    else
//        echoServPort = 7; /* 7 is the well-known port for the echo service */
//    
//    
    
    
    
    
    //TODO: setup and listen
    //setupAndListenSocket(4000);
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
                //TODO: fix this hardcoded mess
//                fflush(stdout);
//                printf("IP address: ");
//                fflush(stdout);
//                getchar();
//                scanf ("%[^\n]%*c", &servIP);
                printf("\nPort: ");
                fflush(stdout);
                scanf("%d", &servPort);
//                servSocket = loginToServer(servIP, servPort);
                servSocket = loginToServer("127.0.0.1", servPort);
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
                //if(servSocket == 0) printf("Connect to server first");
                continue;
                 break;
            case 4:
                //if(servSocket == 0) printf("Connect to server first");
                continue;
                break;
            case 5:
                //if(servSocket == 0) printf("Connect to server first");
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
