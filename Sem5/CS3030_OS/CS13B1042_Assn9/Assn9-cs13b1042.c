/* Required header files */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

int capacity, prod_threads, cons_threads, producer_loops, consumer_loops, avg1, avg2;
  
int *buffer;
int buffer_index;
  
pthread_mutex_t buffer_mutex;

sem_t full;  /* when 0, buffer is full */
sem_t empty; /* when 0, buffer is empty. Kind of
                    like an index for the buffer */

//function to calculate exponential distribution around mean
int ex(int a)
{
    int var = (-1)*((rand() % 10) + 1)/a;
    int x = 100* (exp(var));
    x = x/a;
    return x;
}

// Insert item
void insertItem(int item)
{
    if (buffer_index < capacity) 
    {
        buffer[buffer_index++] = item;
    } 
    else 
    {
        printf("Buffer overflow\n");
    }
}

// For delete item  
int removeItem() 
{
    if (buffer_index > 0) 
    {
        return buffer[--buffer_index]; // buffer_index-- would be error!
    } 
    else 
    {
        printf("Buffer underflow\n");
    }
    return 0;
}
  
long long total_prod = 0;
struct timeval timevar;

//producer 
void *producer(void *param) 
{
    struct tm *prodTime;    //for printing time
    
    gettimeofday(&timevar, NULL);
    long long t1 = timevar.tv_usec;

    int item, pro_thr_num, i;
    pro_thr_num = *(int *)param;

    for(i=0; i<producer_loops; i++) 
    {
        
        item = rand() % 100;
        pthread_mutex_lock(&buffer_mutex);      //lock
        do {
            
            pthread_mutex_unlock(&buffer_mutex);    //enters critical section
            
            sem_wait(&full); //wait if it is full
            //if another thread uses the buffer wait
            pthread_mutex_lock(&buffer_mutex);
        } while (buffer_index == capacity); // check for spurios wake-ups
        
        insertItem(item);
        time_t current;
        time(&current);

        prodTime = localtime(&current);

        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&empty); // post (increment) emptybuffer semaphore
        printf("%dth item produced by thread %d at %s into buffer location %d, item is %d\n", i, pro_thr_num, asctime(prodTime), buffer_index, item);
        sleep(ex(avg1));
    }
    
    gettimeofday(&timevar, NULL);
    long long t2 = timevar.tv_usec;

    total_prod += (t2-t1)/producer_loops;
    //printf("prod %lld ", total_prod);
    pthread_exit(0);
}

long long total_cons = 0;

void *consumer(void *param) 
{
    struct tm *consTime;
    //struct timevar timeval;

    gettimeofday(&timevar, NULL);
    long long t1 = timevar.tv_usec;

    int item, cons_thr_num, i;
    cons_thr_num = *(int *)param;
    
    for(i=0; i<consumer_loops; i++) 
    {
        pthread_mutex_lock(&buffer_mutex);  //lock
        do 
        {
            pthread_mutex_unlock(&buffer_mutex);    //unlock
            sem_wait(&empty);   //wait if it's empty
            pthread_mutex_lock(&buffer_mutex);  

        } while (buffer_index == 0); //check for spurios wakeups
        
        item = removeItem(item);
        
        time_t current;
        time(&current);
        consTime = localtime(&current);

        pthread_mutex_unlock(&buffer_mutex);
        sem_post(&full); // post (increment) fullbuffer semaphore
        printf("%dth item read from buffer by thread %d at %s from buffer location %d, item is %d\n", i, cons_thr_num, asctime(consTime), buffer_index, item);
        
        sleep(ex(avg2));
   }
    gettimeofday(&timevar, NULL);
    long long t2 = timevar.tv_usec;

    total_cons += (t2-t1)/consumer_loops;
    //printf("cons %lld ", total_cons);
    
    pthread_exit(0);
}
  
int main(int argc, char *argv[]) 
{
    buffer_index = 0;   //buffer index

    FILE *fp;
    fp = fopen("input.txt", "r");

    if(fp == NULL)
    {
        printf("error in opening the file\n"); 
        exit(-1);
    }
    //read input from file and proceed
    fscanf(fp, "%d %d %d %d %d %d %d", &capacity, &prod_threads, &producer_loops, &cons_threads, &consumer_loops, &avg1, &avg2);
    buffer = malloc(capacity * sizeof(int));    //read input from files
    
    pthread_mutex_init(&buffer_mutex, NULL);
    sem_init(&full, 0, capacity); //sem full with max capacity
    sem_init(&empty, 0, 0);         //sem empty initialized to 0
    
    pthread_t prod_thread[prod_threads];
    pthread_t cons_thread[cons_threads];

    int prod_thr_num[prod_threads];
    int cons_thr_num[cons_threads];

    int i;

    for(i=0; i<prod_threads; i++)   //creating producer threads
    {
        prod_thr_num[i] = i;
        pthread_create(&prod_thread[i], NULL, producer, &prod_thr_num[i]);
    }    
    
    for(i=0; i<cons_threads; i++)   //creating consumer threads
    {
        cons_thr_num[i] = i;
        pthread_create(&cons_thread[i], NULL, consumer, &cons_thr_num[i]);
    }

    for (i = 0; i < prod_threads; i++)
        pthread_join(prod_thread[i], NULL);

    for(i=0; i<cons_threads; i++)
        pthread_join(cons_thread[i], NULL);
  
    pthread_mutex_destroy(&buffer_mutex);
    sem_destroy(&full);
    sem_destroy(&empty);
    
    printf("avg time taken by producer thread is %lld usec\n", total_prod/(prod_threads));
    printf("avg time taken by consumer thread is %lld usec\n", total_cons/(cons_threads));
    return 0;
}