#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

sem_t rw_mutex;
sem_t mutex;
int read_count = 0;
int shared_data = 0;
void *reader(void *arg) {
int id = *(int *)arg;
sem_wait(&mutex);
read_count++;
if (read_count == 1) {
sem_wait(&rw_mutex);
}
sem_post(&mutex);
printf("Reader %d is reading data: %d\n", id, shared_data);
sem_wait(&mutex);
read_count--;
if (read_count == 0) {
sem_post(&rw_mutex);
}
sem_post(&mutex);
return NULL;
}
void *writer(void *arg) {
int id = *(int *)arg;
sem_wait(&rw_mutex);
shared_data++;
printf("Writer %d is writing data: %d\n", id, shared_data);
sem_post(&rw_mutex);
return NULL;
}
int main() {
int num_readers, num_writers;
printf("Enter the number of readers: ");
scanf("%d", &num_readers);
printf("Enter the number of writers: ");
scanf("%d", &num_writers);
pthread_t readers[num_readers], writers[num_writers];
int reader_ids[num_readers], writer_ids[num_writers];
sem_init(&rw_mutex, 0, 1);
sem_init(&mutex, 0, 1);
for (int i = 0; i < num_readers; i++) {
reader_ids[i] = i + 1;
pthread_create(&readers[i], NULL, reader, &reader_ids[i]);
}
for (int i = 0; i < num_writers; i++) {
writer_ids[i] = i + 1;
pthread_create(&writers[i], NULL, writer, &writer_ids[i]);
}
for (int i = 0; i < num_readers; i++) {
pthread_join(readers[i], NULL);
}
for (int i = 0; i < num_writers; i++) {
pthread_join(writers[i], NULL);
}
sem_destroy(&rw_mutex);
sem_destroy(&mutex);
return 0;
}
