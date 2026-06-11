#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

struct thread_info 
{
    pthread_mutex_t * lock;
    int thread_number;
    pthread_t thread_id;
};

int unsafe_counter = 0;
int safe_counter = 0;

void* foo(void * arg)
{
    unsafe_counter++;
    struct thread_info * info = (struct thread_info*) arg; 
    //pthread_mutex_lock(info->lock);
    safe_counter+=100;
    printf("Current thread number: %d\n",info->thread_number);
    //pthread_mutex_unlock(info->lock);
    return NULL;
}

int main() {
    struct thread_info thread1, thread2, thread3, thread4;
    thread1.thread_number = 1;
    thread2.thread_number = 2;
    thread3.thread_number = 3;
    thread4.thread_number = 4;

    pthread_mutex_t lock;
    thread1.lock = &lock;
    thread2.lock = &lock;
    thread3.lock = &lock;
    thread4.lock = &lock;

    pthread_create(&thread1.thread_id, NULL, foo, &thread1);
    pthread_create(&thread2.thread_id, NULL, foo, &thread2);
    pthread_create(&thread3.thread_id, NULL, foo, &thread3);
    pthread_create(&thread4.thread_id, NULL, foo, &thread4);

    pthread_join(thread1.thread_id, NULL);
    pthread_join(thread2.thread_id, NULL);
    pthread_join(thread3.thread_id, NULL);
    pthread_join(thread4.thread_id, NULL);

    printf("main thread finished. \n");

    printf("safe counter: %d\n", safe_counter);
    printf("unsafe counter: %d\n", unsafe_counter);

    return 0;

}

