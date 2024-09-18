#include <stdio.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>
#include <stdlib.h>

int main(){
    int semid = semget(0xa, 4, IPC_CREAT|IPC_EXCL|0660);
    if (semid == -1) semid = semget(0xa,0,0);
    printf("semid = %d\n", semid);

    semctl(semid, 0, SETVAL, 0); //Sa
    semctl(semid, 1, SETVAL, 1); //Sb
    semctl(semid, 2, SETVAL, 0); //Sc
    semctl(semid, 3, SETVAL, 1); //Sx

    printf("Semaforo %d = %d\n", 0, semctl(semid, 0, GETVAL));
    printf("Semaforo %d = %d\n", 1, semctl(semid, 1, GETVAL));
    printf("Semaforo %d = %d\n", 2, semctl(semid, 2, GETVAL));
    printf("Semaforo %d = %d\n", 3, semctl(semid, 3, GETVAL));

    FILE * f = fopen("log.txt", "a");
    fclose(f);

    exit(0);

}