#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include "semOps.h"

int main(){
    
    int shmContador = shmget(0xa, 0, 0);
    int semId = semget(0xa,0,0);
    
    printf("Contador: %d\n", shmContador);   
   
    int * dirContador = (int *) shmat(shmContador, NULL, 0);
    *dirContador = 0;
    sleep(5);
    printf("inicio pa\n");
    for (int i = 0; i < 10000000; i++)
    {
        
        p(semId, 0);
        *dirContador = *dirContador + 1;
        v(semId, 0);
        if (i == 10) {printf("anda\n");}
    }
    printf("Contador = %d", *dirContador);
    printf("fin pa\n");
}