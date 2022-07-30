/*
 * Author : Cagatay KARASU
 * Since  : 06.06.2021
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#define NO_OF_THREADS 10

struct Singleton {
   char *Data;  
};


struct Singleton *singletonObjectPointer;


int addresses[NO_OF_THREADS];
sem_t sem;

void *runner(void *params); /* the thread */
struct Singleton *getInstance();

int main()
{
        int i;
        sem_init(&sem,0,1);
        pthread_t threadIds[NO_OF_THREADS];
        

        for (i=0; i < NO_OF_THREADS; i++){
                pthread_create(&threadIds[i], NULL, &runner, (void *)(i));
        } 

        /* Wait until all threads are done */
        for (i=0; i < NO_OF_THREADS; i++){
                pthread_join(threadIds[i], NULL);
        } 
        
        /* Control addresses. All of them should be same */
        int prevAddr=addresses[0];
        for (i=1; i < NO_OF_THREADS; i++){
                if(addresses[i]!=prevAddr){
                        printf("Object is created more than once\n");
                        return -1;
                }
                prevAddr=addresses[i];
        }
        for (i=0; i < NO_OF_THREADS; i++){
                printf("Singleton Addresses for each thread %x\n",addresses[i]);
        }
//      printf("Successful\n");
        return 1;
}

/**
 * The thread will begin control in this function
 */
void *runner(void *params) 
{
        int i = (int)params;
        printf("Thread %d\n",i);
        struct Singleton *s = getInstance(i);
        addresses[i]=s;
        int pthread_mutex_unlock(pthread_mutex_t *mutex);
        pthread_exit(0);
}

//Fill this method
struct Singleton *getInstance()
{
    int value; // (To control semaphore, read to value of semaphore)
    
    sem_getvalue(&sem, &value); // (if value is 0. there is struct type previously)
    
    // Array control and object demonstrates same block
    if (value == 0){
        for( int p = 0; p < NO_OF_THREADS; p++){
            if(addresses[p] !=0){
                singletonObjectPointer = (struct Singleton *)malloc(sizeof(struct Singleton));
                singletonObjectPointer = addresses[p];
                return singletonObjectPointer;
            }
        }
    }

    // if value is non zero, semaphore will close after that object will be created
    sem_wait(&sem);
    
    
    singletonObjectPointer = (struct Singleton *)malloc(sizeof(struct Singleton));
    printf("---Address of singletonObjectPointer is %x\n",singletonObjectPointer);
    singletonObjectPointer->Data="This is object data"; 

    return singletonObjectPointer;
}
