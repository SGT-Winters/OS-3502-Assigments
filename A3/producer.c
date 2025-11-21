//Name: Rafay Hassan
//Class: OS-3502/04 Christopher Regan
//Assignment: 3

#include "buffer.h"

shared_buffer_t* buffer = NULL;
sem_t* mutex = NULL;
sem_t* empty = NULL;
sem_t* full  = NULL;
sem_t* print = NULL;
int shm_id = -1;

void cleanup() {
    if (buffer != NULL)
        shmdt(buffer);

    if (mutex != SEM_FAILED) sem_close(mutex);
    if (empty != SEM_FAILED) sem_close(empty);
    if (full  != SEM_FAILED) sem_close(full);
    if (print != SEM_FAILED) sem_close(print);
}

void signal_handler(int sig) {
    sem_wait(print);
    printf("\nProducer: Caught signal %d. Cleaning up...\n", sig);
    fflush(stdout);
    sem_post(print);

    cleanup();
    exit(0);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <producer_id> <num_items>\n", argv[0]);
        exit(1);
    }

    int producer_id = atoi(argv[1]);
    int num_items   = atoi(argv[2]);

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    srand(time(NULL) + producer_id);

    shm_id = shmget(SHM_KEY, sizeof(shared_buffer_t), IPC_CREAT | 0666);
    if (shm_id < 0) {
        perror("shmget failed");
        exit(1);
    }

    buffer = (shared_buffer_t*) shmat(shm_id, NULL, 0);
    if (buffer == (void*) -1) {
        perror("shmat failed");
        exit(1);
    }

        buffer->head = 0;
        buffer->tail = 0;
        buffer->count = 0;

    mutex = sem_open(SEM_MUTEX, O_CREAT, 0644, 1);
    empty = sem_open(SEM_EMPTY, O_CREAT, 0644, BUFFER_SIZE);
    full  = sem_open(SEM_FULL,  O_CREAT, 0644, 0);
    print = sem_open(SEM_PRINT, O_CREAT, 0644, 1);

    if (mutex == SEM_FAILED || empty == SEM_FAILED || full == SEM_FAILED || print == SEM_FAILED) {
        perror("sem_open failed");
        exit(1);
    }
    
    for (int i = 0; i < num_items; i++) {
        item_t item;
        item.value = producer_id * 1000 + i;
        item.producer_id = producer_id;

        sem_wait(empty);
        sem_wait(mutex);

        buffer->buffer[buffer->head] = item;
        buffer->head = (buffer->head + 1) % BUFFER_SIZE;
        buffer->count++;
        
        //To help me solve printing issues, I used Github Copilot to suggest fixes during debugging. All final code was written and verified by me.
        sem_wait(print);
        printf("Producer %d: Produced value %d\n", producer_id, item.value);
        fflush(stdout);
        sem_post(print);

        sem_post(mutex);
        sem_post(full);

        usleep(rand() % 100000);
    }

    sem_wait(print);
    printf("Producer %d: Finished.\n", producer_id);
    fflush(stdout);
    sem_post(print);

    cleanup();
    return 0;
}

