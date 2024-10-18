#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

int main(){
    
    int shmContador = shmget(0xa, 0, 0);

    
    printf("Contador: %d\n", shmContador);   
   
    int * dirContador = (int *) shmat(shmContador, NULL, 0);
    printf("Contador = %d", *dirContador);
}