#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include "semOps.h"

char consumir[50];

int main(){
    
    int shmContador = shmget(0xa, 0, 0);
    int shmBuffer = shmget(0xb, 0, 0);

    int semid = semget(0xa,0,0);
    
    printf("ContadorID: %d\n", shmContador);
    printf("BufferID: %d\n", shmBuffer);
    printf("SemID: %d\n", semid);      
   
    int * dirContador = (int *) shmat(shmContador, NULL, 0);
    char * dirBuffer = (char *) shmat(shmBuffer,NULL,0);
    int flag = 0;
    int pos = 0;
    while ((!flag) != *dirContador){
        while(*dirContador == 0);
        p(semid, 0);
        strcpy(consumir, dirBuffer[pos]);
        *dirContador = *dirContador - strlen(consumir);
        v(semid, 0);
        printf("%s", consumir);
        fflush(stdout);
        sleep(1);
        pos = (pos + strlen(consumir)) % BUFFERSIZE;
        if (strcmp(consumir, "chau") == 0) flag = 1;
    }
    printf("\n");
    printf("Contador = %d", *dirContador);
    printf("fin pb\n");
    
}