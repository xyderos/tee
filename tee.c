#ifndef _REENTRANT
#define _REENTRANT
#endif
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#define MAXWORKERS 3

pthread_mutex_t barrier;

pthread_cond_t go;

pthread_mutex_t lock1;

pthread_mutex_t lock2;

char *buffer;

FILE *file;

bool cond = true;

bool S1 = true;

bool S2 = true;

bool W1=false;

bool W2=false;

void *Producer(void *arg){

    while(cond){

        W1 = false;

        W2 = false;

        if (feof(stdin)){
            cond = false;
            break;
        }
        while(fgets(buffer ,10,stdin)==NULL && cond){
            if (feof(stdin)){
                cond = false;
                break;
            }
        }

        S1 = false;

        S2 = false;

        while(!(W1 && W2)){}
    }
}
void *Worker1(void *arg){

    while(cond){

        while(S1){}

        if (!feof(stdin)){
            fprintf(file, "%s", buffer);
            fflush(file);
        }
        S1 = true;
        W1 = true;
    }
}
void *Worker2(void *arg){

    while(cond){

        while(S2){}

        if (!feof(stdin)) printf("%s",buffer);

        S2 = true;

        W2 = true;
    }
}

int main(int argc, char *argv[]) {

    file = fopen(argv[1], "w");

    buffer =(char*) malloc(sizeof(char) * 256);

    pthread_attr_t attr;

    pthread_t workerid[MAXWORKERS];

    pthread_attr_init(&attr);

    pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

    pthread_mutex_init(&lock1, NULL);

    pthread_mutex_init(&lock2, NULL);

    pthread_mutex_init(&barrier, NULL);

    pthread_cond_init(&go, NULL);

    pthread_create(&workerid[1], &attr, Producer, NULL);

    pthread_create(&workerid[2], &attr, Worker1,NULL);

    pthread_create(&workerid[3], &attr, Worker2,NULL);

    pthread_join(workerid[1], NULL);

    pthread_join(workerid[2], NULL);

    pthread_join(workerid[3], NULL);

    pthread_exit(NULL);
}