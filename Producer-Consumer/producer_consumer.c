#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


#define BUFFER_SIZE 5
typedef int buffer_item;
buffer_item buffer[BUFFER_SIZE];


int in = 0;
int out = 0;


sem_t empty;
sem_t full;
pthread_mutex_t mutex;

int insert_item(buffer_item item) {

    buffer[in] = item;
    in = (in + 1) % BUFFER_SIZE;
    return 0;
}


int remove_item(buffer_item *item) {
    *item = buffer[out];
    out = (out + 1) % BUFFER_SIZE;
    return 0;
}

void *producer(void *param) {
    buffer_item item;
    int thread_id = *((int *)param);

    while (1) {
        
        sleep(rand() % 3 + 1);

        
        item = rand() % 100;

        
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);

        if (insert_item(item) == 0) {
            printf("Producer %d produced: %d\n", thread_id, item);
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&full);
    }
}


void *consumer(void *param) {
    buffer_item item;
    int thread_id = *((int *)param);

    while (1) {
        
        sleep(rand() % 3 + 1);

       
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        if (remove_item(&item) == 0) {
            printf("Consumer %d consumed: %d\n", thread_id, item);
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <sleep_time> <num_producers> <num_consumers>\n", argv[0]);
        return 1;
    }

    
    int sleep_time = atoi(argv[1]);
    int num_producers = atoi(argv[2]);
    int num_consumers = atoi(argv[3]);

    
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    int producer_ids[num_producers];
    int consumer_ids[num_consumers];
    
   
    pthread_t producers[num_producers];
    pthread_t consumers[num_consumers];

   
    for (int i = 0; i < num_producers; i++) {
        producer_ids[i] = i;
        pthread_create(&producers[i], NULL, producer, &producer_ids[i]);
    }


    for (int i = 0; i < num_consumers; i++) {
        consumer_ids[i] = i;
        pthread_create(&consumers[i], NULL, consumer, &consumer_ids[i]);
    }


    sleep(sleep_time);

    printf("Main thread exiting...\n");

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}