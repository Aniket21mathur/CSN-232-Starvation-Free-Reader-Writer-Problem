#include<pthread.h>
# include<bits/stdc++.h>
#include<semaphore.h>
#include<stdio.h>
using namespace std;


void *reader(void *);
void *writer(void *);

// common variables to the threads
int shared_var = 10, r_counter = 0;

sem_t rcnt; // semaphore used in updating r_counter
sem_t rdwr;  //semaphores used to check if readers/writers are allowed to the critical section or not.
sem_t addt; // an additional semaphore to avoid priority to the readers and hance starvation. 

// Storing thread ids assuming max 20 readers and 5 swriters. 
pthread_t r[20],w[5];


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


int main(){
    //intitialising semaphores
    sem_init(&rcnt,0,1);
    sem_init(&rdwr,0,1);
    sem_init(&addt,0,1);


    //threads
    pthread_create(&r[0],NULL,reader,(void *)0);
    pthread_create(&r[1],NULL,reader,(void *)1);
    pthread_create(&r[2],NULL,reader,(void *)2);
    pthread_create(&r[3],NULL,reader,(void *)3);
    pthread_create(&w[0],NULL,writer,(void *)0);
    pthread_create(&r[4],NULL,reader,(void *)4);
    pthread_create(&r[5],NULL,reader,(void *)5);
    pthread_create(&r[6],NULL,reader,(void *)6);
    pthread_create(&w[1],NULL,writer,(void *)1);
    pthread_create(&w[2],NULL,writer,(void *)2);
    pthread_create(&r[7],NULL,reader,(void *)7);
    pthread_create(&r[8],NULL,reader,(void *)8);
    pthread_create(&r[9],NULL,reader,(void *)9);
    pthread_create(&r[10],NULL,reader,(void *)10);
    pthread_create(&r[11],NULL,reader,(void *)11);
    pthread_create(&r[12],NULL,reader,(void *)12);
    pthread_create(&r[13],NULL,reader,(void *)13);
    pthread_create(&w[3],NULL,writer,(void *)3);
    pthread_create(&r[14],NULL,reader,(void *)14);
    pthread_create(&r[15],NULL,reader,(void *)15);
    pthread_create(&r[16],NULL,reader,(void *)16);
    pthread_create(&r[17],NULL,reader,(void *)17);
    pthread_create(&r[18],NULL,reader,(void *)18);
    pthread_create(&r[19],NULL,reader,(void *)19);
    pthread_create(&w[4],NULL,writer,(void *)4);       
    
    //waiting for the threads to finish
    pthread_join(r[0],NULL);
    pthread_join(r[1],NULL);
    pthread_join(r[2],NULL);
    pthread_join(r[3],NULL);
    pthread_join(w[0],NULL);
    pthread_join(r[4],NULL);
    pthread_join(r[5],NULL);
    pthread_join(r[6],NULL);
    pthread_join(w[1],NULL);
    pthread_join(w[2],NULL);    
    pthread_join(r[7],NULL);
    pthread_join(r[8],NULL);
    pthread_join(r[9],NULL);
    pthread_join(r[10],NULL);
    pthread_join(r[11],NULL);
    pthread_join(r[12],NULL);
    pthread_join(r[13],NULL);
    pthread_join(w[3],NULL);    
    pthread_join(r[14],NULL);
    pthread_join(r[15],NULL);
    pthread_join(r[16],NULL);
    pthread_join(r[17],NULL);
    pthread_join(r[18],NULL);
    pthread_join(r[19],NULL);
    pthread_join(w[4],NULL);

return 0;
}
