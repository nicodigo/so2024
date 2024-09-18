#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>


int main(){
    int shmBuffer = shmget(0xa, 0, 0);
    int shmContador = shmget(0xb, 0, 0);

    printf("Buffer: %d\n", shmBuffer);
    printf("Contador: %d\n", shmContador);   

    char * dirbuffer = (char *) shmat(shmBuffer, NULL, 0);
    int * dirContador = (int *) shmat(shmContador, NULL, 0);

    strcpy(dirbuffer, ".....");
    *dirContador = 0;

    exit(0);    

}