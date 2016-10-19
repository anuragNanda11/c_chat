//
//  SetupAndListenSocket.h
//  ChatProgram
//
//  Created by user on 10/10/16.
//  Copyright © 2016 user. All rights reserved.
//

#ifndef coreDataAndFunctions_h
#define coreDataAndFunctions_h

#include <stdio.h>
#include <stdbool.h>


#define MAXPENDING 5 /* Maximum outstanding connection requests */

void DieWithError(char *errorMessage); /* Error handling function */

void setupAndListenSocket(unsigned short port);

extern char* hello;

struct uac{
    char username[12];
    char password[12];
    char messages[150];
    size_t idx_freespc;
    size_t num_msgs;
};

typedef struct uac userAccount;

#define NUM_USERS 5
extern userAccount g_accounts[NUM_USERS];

extern size_t g_currUsrId;

#endif /* SetupAndListenSocket_h */


