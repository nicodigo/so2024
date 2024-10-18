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

    int ind = 0;
    char letra = 'A';
    while(1){
        while(*dirContador == 5); // bueffer lleno
        //seccion critica
        dirbuffer[ind] = letra;
        *dirContador = *dirContador + 1;
        //fin seccion critica
        printf("produce: %c pos: %d\n", dirbuffer[ind], ind);
        ind = (ind + 1) % 5;
        
        if (letra == 'Z'){
            printf("Termina Productor\n");
            exit(0);
        }else{
            letra++;
        }


    }
}