#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>


int main(){
    int shmBuffer = shmget(0xa, sizeof(char)*5, IPC_CREAT|IPC_EXCL|0600);
    int shmContador = shmget(0xb, sizeof(int), IPC_CREAT|IPC_EXCL|0600);

    printf("Buffer: %d\n", shmBuffer);
    printf("Contador: %d\n", shmContador);   

    exit(0);
}