#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

unsigned int value1, value2, count;
pthread_mutex_t  mutex;
pthread_rwlock_t rwlock;
void *function(void *arg);

int main(int argc,  char *argv[])
{
    pthread_t  a_thread;

    if(pthread_mutex_init(&mutex, NULL) < 0){
        perror("fail to mutex init!\n");
        exit(-1);
    }

    if(pthread_rwlock_init(&rwlock,NULL) < 0){
            perror("fail to rwlock init!\n");
            exit(-1);
    }

    if(pthread_create(&a_thread,NULL,function,NULL) < 0){
        perror("fail to pthread_create");
        exit(-1);
    }

    while(1){

        //pthread_mutex_trylock(&mutex);
        pthread_rwlock_trywrlock(&rwlock);

        count++;
        value1 = count;
        value2 = count;

        pthread_rwlock_unlock(&rwlock);
      //  pthread_mutex_unlock(&mutex);
    }

pthread_mutex_destory(&mutex);

    return 0;
}

void *function(void *arg)
{
    while(1){
        pthread_rwlock_tryrdlock(&rwlock);
        if(value1 != value2){
            printf("count = %d,value1 = %d, value2 = %d\n",count,value1,value2);
        }
     pthread_rwlock_unlock(&rwlock);
    }
}
