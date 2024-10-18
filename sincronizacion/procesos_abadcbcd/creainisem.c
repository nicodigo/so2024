#include <stdio.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h>
#include "semDefs.h"

int main(){
    int semid = semget(key, cantSem, IPC_CREAT|IPC_EXCL|0660);
    if (semid == -1) semid = semget(key,0,0);
    printf("semid = %d\n", semid);

    semctl(semid, Sa, SETVAL, 1); 
    semctl(semid, Sb, SETVAL, 1); 
    semctl(semid, Sc, SETVAL, 0); 
    semctl(semid, Sd, SETVAL, 0); 
     semctl(semid, Sx, SETVAL, 0); 
    semctl(semid, Si1, SETVAL, 1); 
    semctl(semid, Si2, SETVAL, 0); 


    printf("Semaforo sa %d = %d\n", Sa, semctl(semid, Sa, GETVAL));
    printf("Semaforo sb %d = %d\n", Sb, semctl(semid, Sb, GETVAL));
    printf("Semaforo sc %d = %d\n", Sc, semctl(semid, Sc, GETVAL));
    printf("Semaforo sd %d = %d\n", Sd, semctl(semid, Sd, GETVAL));
    printf("Semaforo sx %d = %d\n", Sx, semctl(semid, Sx, GETVAL));
    printf("Semaforo si1 %d = %d\n", Si1, semctl(semid, Si1, GETVAL));
    printf("Semaforo si2 %d = %d\n", Si2, semctl(semid, Si2, GETVAL));
    

 

    exit(0);

}