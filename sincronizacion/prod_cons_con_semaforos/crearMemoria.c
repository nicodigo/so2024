#include <stdio.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/shm.h>
#include "semOps.h"

int main(){
    //creacion e inicializacion del semaforo
    int semid = semget(0xa, 1, IPC_CREAT|IPC_EXCL|0660);
    
    if (semid == -1) semid = semget(0xa,0,0);
    printf("semid = %d\n", semid);

    semctl(semid, 0, SETVAL, 1); //Sem
    

    printf("Semaforo %d = %d\n", 0, semctl(semid, 0, GETVAL));


    //creacion e inicializacion de la memoria compartida 
    int shmContador = shmget(0xa, sizeof(int), IPC_CREAT|IPC_EXCL|0600);
    if (shmContador == -1) shmContador = shmget(0xa,0,0);

    printf("ContadorID: %d\n", shmContador);   

    int * dirContador = (int *) shmat(shmContador, NULL, 0);
    *dirContador = 0;

    int shmBuffer = shmget(0xb, BUFFERSIZE, IPC_CREAT|IPC_EXCL|0600);
    if (shmBuffer == -1) shmget(0xb, 0,0);

    printf("BufferID: %d\n", shmBuffer);


    exit(0);
}