# Starvation-Free-Reader-Writer-Problem Solution
## Aniket Mathur(17116008)

The core code is in a .cpp file named **starvationfree.cpp**. The program can be executed by runing the above cpp file.

There are two functions reader and writer. It is considered that there are 20 reader threads and 5 writer threads. The **pthread.h** library of C++ is used to generate threads. The reader and writer threads reads/writes a global variables `shared_var`. Semaphores used are `rcnt`, `addt` and `rdwr`.

# Semaphores
Semaphore is simply a variable that is non-negative and shared between threads. A semaphore is a signaling mechanism, and a thread that is waiting on a semaphore can be signaled by another thread. It uses two atomic operations, 1)wait, and 2) signal for the process synchronization. The following sempahores are used here:

- addt - Using this we can modify the reader-priority solution to equal priority. It used to signal other reader/writer threads that one of the threads is working. This is intialised to 1.
- rcnt - This semaphore is used to maintain mutual exclution amoung readers trying to update `r_counter` variable.
- rdwr - This semaphore is used check if readers/writers are allowed to the critical section or not.

## Reader
An integer variable `r_counter`(intial=0) is used to count the no. of readers in the queue. Reader function:
```
//READER
void *reader(void *i){

    //ENTRY_SECTION
    
    //waiting for other reader/writer to release     
    sem_wait(&addt);

    sem_wait(&rcnt);
    r_counter++;
    if(r_counter==1)
        sem_wait(&rdwr); //wait if writer is writing 

    printf("Reader-%d is in the ENTRY_SECTION\n",i);
    
    sem_post(&rcnt);
    sem_post(&addt);

    //CRITICAL_SECTION

    printf("Reader-%d is reading the value = %d\n",i,shared_var);

    //EXIT_SECTION

    sem_wait(&rcnt);
    r_counter--;
    if(r_counter==0)
        sem_post(&rdwr);
    
    printf("Reader-%d is in the EXIT_SECTION\n",i);

    sem_post(&rcnt);

}
```
The diffence between this solution and the reader-priority solution is the use of an additional semaphore `addt`  to avoid priority to the readers so that writers don't starve. 

## Writer

A writer increase the value of shared_var by 15. Writer function: 
```
// WRITER
void *writer(void *i){

    //ENTRY_SECTION

    //wait for currently executing reader to finish updating the r_counter or writer thread to finish 
    sem_wait(&addt); 

    printf("writer-%d is in the ENTRY_SECTION\n",i);

    //wait if some reader is reading/ writer is writing
    sem_wait(&rdwr);

    //CRITICAL_SECTION

    shared_var +=15;
    printf("writer-%d is writing value= %d \n",i,shared_var);

    //EXIT_SECTION

    printf("writer-%d is in the EXIT_SECTION\n",i);

    sem_post(&rdwr);
    sem_post(&addt);
}
```
Similar to reader code only change is semaphore `addt`.

# Sample Output

```
Reader-0 is in the ENTRY_SECTION
Reader-0 is reading the value = 10
Reader-1 is in the ENTRY_SECTION
Reader-0 is in the EXIT_SECTION
Reader-1 is reading the value = 10
Reader-2 is in the ENTRY_SECTION
Reader-1 is in the EXIT_SECTION
Reader-2 is reading the value = 10
Reader-3 is in the ENTRY_SECTION
Reader-3 is reading the value = 10
Reader-2 is in the EXIT_SECTION
writer-0 is in the ENTRY_SECTION
writer-0 is writing value= 25 
Reader-3 is in the EXIT_SECTION
writer-0 is in the EXIT_SECTION
Reader-4 is in the ENTRY_SECTION
Reader-5 is in the ENTRY_SECTION
Reader-4 is reading the value = 25
Reader-5 is reading the value = 25
Reader-6 is in the ENTRY_SECTION
Reader-6 is reading the value = 25
Reader-4 is in the EXIT_SECTION
writer-1 is in the ENTRY_SECTION
Reader-5 is in the EXIT_SECTION
Reader-6 is in the EXIT_SECTION
writer-1 is writing value= 40
writer-1 is in the EXIT_SECTION
writer-2 is in the ENTRY_SECTION
writer-2 is writing value= 55
writer-2 is in the EXIT_SECTION
Reader-7 is in the ENTRY_SECTION
Reader-8 is in the ENTRY_SECTION
Reader-8 is reading the value = 55
Reader-7 is reading the value = 55
Reader-9 is in the ENTRY_SECTION
Reader-8 is in the EXIT_SECTION
Reader-9 is reading the value = 55
Reader-7 is in the EXIT_SECTION
Reader-10 is in the ENTRY_SECTION
Reader-9 is in the EXIT_SECTION
Reader-10 is reading the value = 55
Reader-11 is in the ENTRY_SECTION
Reader-11 is reading the value = 55
Reader-10 is in the EXIT_SECTION
Reader-12 is in the ENTRY_SECTION
Reader-12 is reading the value = 55
Reader-11 is in the EXIT_SECTION
Reader-13 is in the ENTRY_SECTION
Reader-12 is in the EXIT_SECTION
writer-3 is in the ENTRY_SECTION
Reader-13 is reading the value = 55
writer-3 is writing value= 70
writer-3 is in the EXIT_SECTION
Reader-13 is in the EXIT_SECTION
Reader-14 is in the ENTRY_SECTION
Reader-15 is in the ENTRY_SECTION
Reader-15 is reading the value = 70
Reader-14 is reading the value = 70
Reader-16 is in the ENTRY_SECTION
Reader-16 is reading the value = 70
Reader-15 is in the EXIT_SECTION
Reader-14 is in the EXIT_SECTION
Reader-17 is in the ENTRY_SECTION
Reader-16 is in the EXIT_SECTION
Reader-17 is reading the value = 70
Reader-18 is in the ENTRY_SECTION
Reader-18 is reading the value = 70
Reader-17 is in the EXIT_SECTION
Reader-19 is in the ENTRY_SECTION
Reader-19 is reading the value = 70
Reader-18 is in the EXIT_SECTION
writer-4 is in the ENTRY_SECTION
writer-4 is writing value= 85
writer-4 is in the EXIT_SECTION
Reader-19 is in the EXIT_SECTION
```

# Observation

From the output it is clear that neither reader nor wirter is given priority. Writers are able to write even if the readers are waiting. Even if there are readers in the queue, the writers enter the entry section. This is not the case with with reader priority solution. Hence writer is able to write even if a reader is in the queue. So the solution is **starvation free**.
