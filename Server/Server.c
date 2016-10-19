#include <stdio.h> /* for printf() and fprintf() */

#include <stdlib.h> /* for atoi() */

#include <unistd.h> /* for close() */

#include "CoreDataAndFunctions.h"

void DieWithError(char *errorMessage); /* Error handling function */
void HandleTCPClient(int clntSocket); /* TCP client handling function */

//global declared in coreDataAndFunctions.
//userAccount g_accounts[NUM_USERS];


void setUpUserAccounts(userAccount accounts[], size_t num )
{
    for (size_t i = 0; i < num; i++) {
        sprintf(accounts[i].username,"user_%zu", i );
        sprintf(accounts[i].password, "pswd%zu", i);
        accounts[i].idx_freespc = 0;
        accounts[i].num_msgs = 0;
    }
}

int main(int argc, char *argv[])
{
    
    setUpUserAccounts(g_accounts, NUM_USERS);
    
    printf(g_accounts[1].username);
    
    if (argc != 2) /* Test for correct number of arguments */
    {
        fprintf(stderr, "Usage: %s <Server Port>\n", argv[0]) ;
        exit(1);
    }

   setupAndListenSocket(atoi(argv[1])); /* First arg: local port */

    
/* NOT REACHED */
}


