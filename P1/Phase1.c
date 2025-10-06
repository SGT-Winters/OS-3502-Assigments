#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_THREADS 3
#define NUM_ACCOUNTS 1
#define TRANSACTIONS_PER_TELLER 5

typedef struct {
	int account_id;
	double balance;
	int transaction_count;
}Account;

Account accounts[NUM_ACCOUNTS];

void* teller_thread(void* arg){
	int teller_id = *(int*)arg;

	for(int i = 0; i < TRANSACTIONS_PER_TELLER; i++){
		int accountIndex = 0;

		int deposit = rand() % 2;
		double amount = (deposit ? 100.0 : 50.0);

		double previousBalance = accounts[accountIndex].balance;
		usleep(100000);

		if(deposit) {
			accounts[accountIndex].balance = previousBalance + amount;
			printf("Thread %d: Depositing %.2f\n", teller_id, amount);
		}else{
			accounts[accountIndex].balance = previousBalance - amount;
			printf("Thread %d: Withdrawing %.2f\n", teller_id, amount);
		}
		accounts[accountIndex].transaction_count++;
	}//end of for statement
	return NULL;
}//end of teller_thread

int main(){
	srand(time(NULL));

	accounts[0].account_id = 1;
	accounts[0].balance = 1000.0;
	accounts[0].transaction_count = 0;

	printf("Initial balance: %.2f\n", accounts[0].balance);
	
	pthread_t threads[NUM_THREADS];
	int thread_ids[NUM_THREADS];

	for(int i = 0; i < NUM_THREADS; i++){
		thread_ids[i] = i + 1;
		pthread_create(&threads[i], NULL, teller_thread, &thread_ids[i]);
	}//end of for statement

	for(int i = 0; i < NUM_THREADS; i++){
		pthread_join(threads[i], NULL);
	}//end of for  statement

	printf("\nFinal balance: %.2f\n", accounts[0].balance);

return 0;
}//end of main
