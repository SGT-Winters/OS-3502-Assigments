//Name: Rafay Hassan
//Class: OS-3502/04 Christopher Regan
//Assignment: 3

#ifndef BUFFER_H
#define BUFFER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <time.h>

#define BUFFER_SIZE 10
#define SHM_KEY 0x1234

#define SEM_MUTEX "/sem_mutex"
#define SEM_EMPTY "/sem_empty"
#define SEM_FULL  "/sem_full"
#define SEM_PRINT "/sem_print" //To help me solve printing issues, I used Github Copilot to suggest fixes during debugging. All final code was written and verified by me.

typedef struct {
    int value;
    int producer_id;
} item_t;

typedef struct {
    item_t buffer[BUFFER_SIZE];
    int head;
    int tail;
    int count;
} shared_buffer_t;

#endif

