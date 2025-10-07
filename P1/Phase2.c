#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_THREADS 3
#define NUM_ACCOUNTS 1
#define TRANSACTIONS_PER_TELLER 5
#define INITIAL_BALANCE 1000.0

typedef struct {
    int account_id;
    double balance;
    int transaction_count;
    pthread_mutex_t lock;
} Account;

Account accounts[NUM_ACCOUNTS];

void deposit_money(int account_id, double amount, int teller_id) {
    pthread_mutex_lock(&accounts[account_id].lock);

    accounts[account_id].balance += amount;
    accounts[account_id].transaction_count++;

    printf("Thread %d: Depositing %.2f | New balance: %.2f\n",
           teller_id, amount, accounts[account_id].balance);

    pthread_mutex_unlock(&accounts[account_id].lock);
} // end of deposit_money

void withdraw_money(int account_id, double amount, int teller_id) {
    pthread_mutex_lock(&accounts[account_id].lock);

    accounts[account_id].balance -= amount;
    accounts[account_id].transaction_count++;

    printf("Thread %d: Withdrawing %.2f | New balance: %.2f\n",
           teller_id, amount, accounts[account_id].balance);

    pthread_mutex_unlock(&accounts[account_id].lock);
} // end of withdraw_money

void* teller_thread(void* arg) {
    int teller_id = *(int*)arg;

    for (int i = 0; i < TRANSACTIONS_PER_TELLER; i++) {
        int accountIndex = 0;
        int depositting = rand() % 2;
        double amount = (depositting ? 100.0 : 50.0);

        if (depositting) {
            deposit_money(accountIndex, amount, teller_id);
        } else {
            withdraw_money(accountIndex, amount, teller_id);
        }

        usleep(100000);
    } //end of for statement

    return NULL;
} // end of teller_thread

int main() {
    srand(time(NULL));
    clock_t start_time = clock();

    for (int i = 0; i < NUM_ACCOUNTS; i++) { 
        accounts[i].account_id = i + 1;
        accounts[i].balance = INITIAL_BALANCE;
        accounts[i].transaction_count = 0;
        pthread_mutex_init(&accounts[i].lock, NULL);
    } //end of for statement

    printf("Initial balance: %.2f\n", accounts[0].balance);

    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i + 1;
        pthread_create(&threads[i], NULL, teller_thread, &thread_ids[i]);
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    clock_t end_time = clock();
    double elapsed = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("\nFinal balance: %.2f (Transactions Done: %d)\n",
           accounts[0].balance, accounts[0].transaction_count);
    printf("Time: %.4f seconds\n", elapsed);

    return 0;
} // end of main
