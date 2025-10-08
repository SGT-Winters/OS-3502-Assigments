#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <stdatomic.h>

#define NUM_ACCOUNTS 3
#define INITIAL_BALANCE 1000.0
#define NUM_THREADS 6
#define CHECKING 2
#define STUCK 5

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

int timed_lock(pthread_mutex_t *mutex, int seconds) {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    ts.tv_sec += seconds;
    return pthread_mutex_timedlock(mutex, &ts);
}

void *transfer(void *arg) {
    int thread_id = ((int *)arg)[2];
    int from_id = ((int *)arg)[0];
    int to_id   = ((int *)arg)[1];

    int first = (from_id < to_id) ? from_id : to_id;
    int second = (from_id < to_id) ? to_id : from_id;

    printf("Thread %d: Attempting transfer from %d to %d\n",
           thread_id, accounts[from_id].account_id, accounts[to_id].account_id);
    fflush(stdout);

    if (timed_lock(&accounts[first].lock, 2) != 0) {
        printf("Thread %d: Failed to lock first account %d, retrying later...\n",
               thread_id, accounts[first].account_id);
        fflush(stdout);
        return NULL;
    }

    printf("Thread %d: Locked first account %d\n",
           thread_id, accounts[first].account_id);
    fflush(stdout);
    update_progress();

    usleep(500000); // simulate delay

    int second_lock = timed_lock(&accounts[second].lock, 2);
    if (second_lock != 0) {
        printf("Thread %d: Timeout waiting for account %d, aborting transfer.\n",
               thread_id, accounts[second].account_id);
        fflush(stdout);
        pthread_mutex_unlock(&accounts[first].lock);
        return NULL;
    }

    printf("Thread %d: Locked second account %d\n",
           thread_id, accounts[second].account_id);
    fflush(stdout);

    // Execute transfer
    accounts[from_id].balance -= 100.0;
    accounts[to_id].balance   += 100.0;
    update_progress();

    pthread_mutex_unlock(&accounts[second].lock);
    pthread_mutex_unlock(&accounts[first].lock);

    printf("Thread %d: Transfer completed successfully.\n", thread_id);
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
            printf("\nWarning: No progress for %ld seconds — possible timeout.\n",
                   (long)difftime(now, last));
            fflush(stdout);
            pthread_exit(NULL);
        }
    }
    return NULL;
}

int main() {
    srand(time(NULL));

    // Initialize all accounts
    for (int i = 0; i < NUM_ACCOUNTS; i++) {
        accounts[i].account_id = 11 + i;
        accounts[i].balance = INITIAL_BALANCE;
        pthread_mutex_init(&accounts[i].lock, NULL);
    }

    atomic_store(&last_progress_time, time(NULL));

    pthread_t threads[NUM_THREADS];
    pthread_t wd;
    int args[NUM_THREADS][3];

    // Start watchdog
    pthread_create(&wd, NULL, detect_report, NULL);

    // Create multiple transfer threads with random pairs
    for (int i = 0; i < NUM_THREADS; i++) {
        int from = rand() % NUM_ACCOUNTS;
        int to = rand() % NUM_ACCOUNTS;
        while (to == from) {
            to = rand() % NUM_ACCOUNTS;
        }

        args[i][0] = from;
        args[i][1] = to;
        args[i][2] = i + 1;

        pthread_create(&threads[i], NULL, transfer, args[i]);
        usleep(100000); // small delay to vary thread timing
    }

    // Wait for all transfers to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Stop watchdog
    pthread_cancel(wd);

    // Final balance report
    double total = 0.0;
    printf("\nFinal Balances:\n");
    for (int i = 0; i < NUM_ACCOUNTS; i++) {
        printf("Account %d = %.2f\n", accounts[i].account_id, accounts[i].balance);
        total += accounts[i].balance;
    }
    printf("Total balance across all accounts = %.2f\n", total);

    // Cleanup
    for (int i = 0; i < NUM_ACCOUNTS; i++) {
        pthread_mutex_destroy(&accounts[i].lock);
    }

    return 0;
}

