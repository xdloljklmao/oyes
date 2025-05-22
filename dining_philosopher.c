#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define NUM_PHILOSOPHERS 5

sem_t forks[NUM_PHILOSOPHERS]; // for sempaphore
pthread_mutex_t mutex;    

void* philosopher(void* num) {
   int id = *(int*)num;

   while (1) {
       printf("Philosopher %d is thinking.\n", id);
       printf("Philosopher %d is hungry.\n", id);

       pthread_mutex_lock(&mutex);
       sem_wait(&forks[id]);       //left fork pick
       printf("Philosopher %d picked the left fork: %d\n", id, id);
       sem_wait(&forks[(id + 1) % NUM_PHILOSOPHERS]); // right fork pick
       printf("Philosopher %d picked the right fork: %d\n", id, (id + 1) % NUM_PHILOSOPHERS);
       pthread_mutex_unlock(&mutex);

       printf("Philosopher %d is eating.\n", id);
/
       sem_post(&forks[id]);     //put down left fork
       sem_post(&forks[(id + 1) % NUM_PHILOSOPHERS]); // pout down right fork
       printf("Philosopher %d has finished eating and put down both forks.\n", id);
   }
}

int main() {
   pthread_t philosophers[NUM_PHILOSOPHERS];
   int philosopher_ids[NUM_PHILOSOPHERS];

   pthread_mutex_init(&mutex, NULL);
   for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
       sem_init(&forks[i], 0, 1);
       philosopher_ids[i] = i;
   }

   for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
       pthread_create(&philosophers[i], NULL, philosopher, &philosopher_ids[i]);
   }

   for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
       pthread_join(philosophers[i], NULL);
   }

   for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
       sem_destroy(&forks[i]);
   }
   pthread_mutex_destroy(&mutex);

   return 0;
}
