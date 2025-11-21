# OS-3502-Assigments

## Table of Contents

- [Assignment 2](#assignment-2)<br>
- [Assignment 3](#assignment-3)<br>
- [Project 1](#project-1)<br>
- [Project 2](#project-2)<br>

## Assignment 2

### Helpful Videos
Parse Command Line Arguments: https://www.youtube.com/watch?v=ZA3QfmabUKg <br>
Allocate Buffer using malloc: https://www.youtube.com/watch?v=n_Se6bt8jM0 <br>
Reading Files in C: https://www.youtube.com/watch?v=Hzg3kCHJcxI<br>
Two Way Communication between processes using Pipes in C: https://www.youtube.com/watch?v=8Q9CPWuRC6o <br>
Time Library in C: https://www.youtube.com/watch?v=Qoed2uBwF_o <br>
Time Your Code: https://www.youtube.com/watch?v=1KQqpiXxvWQ
#### Signal Handling 
Handling Signals: https://www.youtube.com/watch?v=jF-1eFhyz1U <br><br>
Communicating Between Processes Using Signals: https://www.youtube.com/watch?v=PErrlOx3LYE&t=268s <br><br>
Sending and Handling Signals in C: https://www.youtube.com/watch?v=83M5-NPDeWs

## Assignment 3

**NOTE:** Clean up shared memory before running new test cases to ensure prints are not out-of-order<br><br>

ipcrm -M 0x1234<br>
rm /dev/shm/sem_*<br>

### Compile Code
gcc -o producer producer.c -pthread -lrt<br>
gcc -o consumer consumer.c -pthread -lrt<br>

## Test Cases
#### Test Case 1
./producer 1 5 & ./consumer 1 5 & wait<br>

![Test 1 Image](A3/images/Test%201.png)

#### Test Case 2
./producer 1 5 & ./producer 1 5 & ./consumer 1 10 & wait<br>

![Test 2 Image](A3/images/Test%202.png)

#### Test Case 3
./producer 1 10 & ./consumer 1 5 & ./consumer 2 5 & wait<br>

![Test 3 Image](A3/images/Test%203.png)

### Helpful Links

* Shared Memory (shmget, shmat) by Jacob Sorber: https://www.youtube.com/watch?v=WgVSq-sgHOc<br>
* How Semaphore Work by Jacob Sorber: youtube.com/watch?v=ukM_zzrIeXs&pp=ygUec2VtYXBob3JlIHN5bmNocm9uaXphdGlvbiBpbiBj<br>
* Intro to Semaphores in C by CodeVault: https://www.youtube.com/watch?v=YSn8_XdGH7c<br><br>
* Quick Overview of Semaphores: https://www.geeksforgeeks.org/operating-systems/semaphores-in-process-synchronization/<br>

## Project 1

### Phase 1
Creates a basic multi-threading banking that allows for accessing shared data between acccounts without any protection<br><br>
**Requirements**
* Create multiple threads
* Each thrad should perform multiple operations
* Display thread IDs and operations details
* Show the race conditions problem

#### Approach
Simulated a simple multi-threading banking system. It uses 3 threads set as the tellers, and each teller does 5 transactions on 1 singular back account shared among them.
The shared account has an account ID, balance, and transaction count. The program randomly decides whether each teller is to deposit or withdraw money when it is their 
time to do a transaction. Every deposit increases the account by $100 while every withdraw decreases the account by $50. A delay is added to help set up a real-world
experience processing time. Every thread accesses and edit the same account at thee same time without any synchronization. Because there are no locks or safe nets in place,
the end balance will be different each time the program is run.

### Phase 2
Add on to Phase 1 but includes mutex locks to protect shared resources and eliminate race conditions, ensuring only 1 thread accesses resource at a time<br><br>
**Requirements**
* Implement pthread mutexes for each account
* Ensure thread-safe access to all shared data
* Verify correct final balances
* Measure performance impact of synchronization

#### Approach
Simulated a multi-threaded banking system that addesses the proper synchronization using mutex locks. Phase 2 implements the locks to prevent multiple threads
from modifying the account at the same time. This ensures that the balance remains consistent and prevents any race conditions. The program also
records the start time and end time between transactions and gives the total time taken for all transactions.

### Phase 3
Add on to Phase 2 but implements account transfers that can cause deadlock.<br><br>
**Requirements**
* Implement transfer operations requiring two account locks
* Create a scenario where deadlock is highly likely to occur
* Detect and report when threads appear stuck (no progress)
* Use multiple threads performing transfers between same accounts

#### Approach
Phase 3 uses mutex locks to protect shared resources and introduces a way to detect deadlocks during runtime. Each thread tries to move money from one
account to another while at the same time acquiring locks on both accounts. I added a delay and a lock ordering that makes it so that each thread waits 
for each other, showcasing a potential deadlock scenario. I then created monitoring system, detect_report, that runs consistently to check the system's
progress. It uses atomic variables to keep track of the last time progress was done by any thread. If it detects that no progress has occurred for more
than a certain time frame, it detects and reports that a deadlock has occurred. It then prints information about the locks and the time it waited.

### Phase 4
Add on to Phase 3 but fixes the problem of deadlocks<br><br>
**Requirements**
* Use Lock Ordering: Always acquire locks in consistent order
* Timeout Mechanism: Use pthread_mutex_timedlock

#### Approach
I created a lock ordering rule, which makes sure that all threads acquire locks at the same sequence. It checks to see which account has the smaller ID 
and always locks that before trying to lock the second one. By doing this, the program removes the chances of a stall or circular wait, which is a key
condition for deadlocks. I then used the function pthread_mutex_timedlock(). This allows a thread to wait only for a certain amount of time when trying
to get a lock. If it cannot get a lock then the thread times out, releases any locks it previously had, and then retries later.

### Helpful Videos
#### Phase 1
* Pthreads in C: https://www.youtube.com/watch?v=ldJ8WGZVXZk<br>
* Race Conditions: https://www.youtube.com/watch?v=K1aoimUYTK8<br>
#### Phase 2
* Mutex: https://www.youtube.com/watch?v=raLCgPK-Igc<br>
#### Phase 3 and 4
* Deadlocks:<br>Video: https://www.youtube.com/watch?v=LjWug2tvSBU<br>
* Deadlock Reading: https://undo.io/resources/detecting-deadlocks-c-cplusplus/<br>

#### LaTeX Tutorial
* https://www.youtube.com/watch?v=y8y_KIs9JLs <br>

## Project 2

### Creating Export Results Feature 

#### Helpful Links:

* Export DataGridView Data to a CSV file in C#: https://www.youtube.com/watch?v=H8zEgfrXJ60&t=8s<br>
* C# Tutorial - How to Export DataGridView to CSV File: https://www.youtube.com/watch?v=UAFXi9sXzyM&t=410s<br>

### Implementing Shortest Remaining Time First (SRTF)

#### Helpful Links:
Explaing and Visualizing SRTF: https://www.youtube.com/watch?v=77_311RNAbg<br>
C approach to SRTF: https://www.youtube.com/watch?v=S6rwMgUuDdc&t=519s&pp=ygUrc2hvcnRlc3QgcmVtYWluaW5nIHRpbWUgZmlyc3Qgc2NoZWR1bGluZyBjIw%3D%3D<br>
