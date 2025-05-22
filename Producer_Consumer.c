
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define BUFFER_SIZE 10
int buffer[BUFFER_SIZE];
int in = 0, out = 0;
sem_t mutex, empty, full;
void* producer(void* arg) {
while (1) {
int item = rand() % 100;
sem_wait(&empty);
sem_wait(&mutex);
buffer[in] = item;
printf("Producer produced: %d\n", item);
in = (in + 1) % BUFFER_SIZE;
sem_post(&mutex);
sem_post(&full);
_sleep(1000);
}
}
void* consumer(void* arg) {
while (1) {
sem_wait(&full);
sem_wait(&mutex);
int item = buffer[out];
printf("Consumer consumed: %d\n", item);
out = (out + 1) % BUFFER_SIZE;
sem_post(&mutex);
sem_post(&empty);
_sleep(2000);
}
}
int main() {
pthread_t producer_thread, consumer_thread;
sem_init(&mutex, 0, 1);
sem_init(&empty, 0, BUFFER_SIZE);
sem_init(&full, 0, 0);
pthread_create(&producer_thread, NULL, producer, NULL);
pthread_create(&consumer_thread, NULL, consumer, NULL);
pthread_join(producer_thread, NULL);
pthread_join(consumer_thread, NULL);
return 0;
}
