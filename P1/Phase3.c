#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdatomic.h>

#define NUM_ACCOUNTS 2
#define INITIAL_BALANCE 1000.0
#define CHECKING 2
#define STUCK  5

typedef struct {
    int account_id;
    double balance;
    pthread_mutex_t lock;
} Account;

Account accounts[NUM_ACCOUNTS];

atomic_long last_progress_time;

void update_progress() {
    atomic_store(&last_progress_time, time(NULL));
}

void *transfer(void *arg) {
    int thread_id = ((int *)arg)[2];
    int from_id = ((int *)arg)[0];
    int to_id   = ((int *)arg)[1];

    printf("Thread %d: Attempting transfer from %d to %d\n",
           thread_id, accounts[from_id].account_id, accounts[to_id].account_id);
    fflush(stdout);

    pthread_mutex_lock(&accounts[from_id].lock);
    printf("Thread %d: Locked account %d\n", thread_id, accounts[from_id].account_id);
    fflush(stdout);

    update_progress();

    usleep(1000000);

    printf("Thread %d: Waiting for account %d\n",
           thread_id, accounts[to_id].account_id);
    fflush(stdout);

    pthread_mutex_lock(&accounts[to_id].lock);
    printf("Thread %d: Locked account %d\n", thread_id, accounts[to_id].account_id);
    fflush(stdout);

    accounts[from_id].balance -= 100.0;
    accounts[to_id].balance   += 100.0;

    update_progress();

    pthread_mutex_unlock(&accounts[to_id].lock);
    pthread_mutex_unlock(&accounts[from_id].lock);

    printf("Thread %d: Transfer completed.\n", thread_id);
    fflush(stdout);
    return NULL;
}

void *detect_report(void *arg) {
    (void)arg;
    while (1) {
        sleep(CHECKING);
        time_t now = time(NULL);
        long last = atomic_load(&last_progress_time);
        if (difftime(now, last) > STUCK) {
            printf("\nDeadlock Hit! No progress for %ld seconds.\n", 
                   (long)difftime(now, last));
            printf("Threads are waiting for each other.\n");
            fflush(stdout);
            pthread_exit(NULL);
        }
    }
    return NULL;
}

int main() {

    accounts[0].account_id = 11;
    accounts[0].balance = INITIAL_BALANCE;
    pthread_mutex_init(&accounts[0].lock, NULL);

    accounts[1].account_id = 12;
    accounts[1].balance = INITIAL_BALANCE;
    pthread_mutex_init(&accounts[1].lock, NULL);

    atomic_store(&last_progress_time, time(NULL));

    pthread_t t1, t2, wd;

    int args1[3] = {0, 1, 1};
    int args2[3] = {1, 0, 2};

    pthread_create(&wd, NULL, detect_report, NULL);
    pthread_create(&t1, NULL, transfer, args1);
    usleep(100000);
    pthread_create(&t2, NULL, transfer, args2);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    pthread_join(wd, NULL);

    printf("\nFinal Balances:\nAccount 11 = %.2f\nAccount 12 = %.2f\n",
           accounts[0].balance, accounts[1].balance);

    pthread_mutex_destroy(&accounts[0].lock);
    pthread_mutex_destroy(&accounts[1].lock);

    return 0;
}
