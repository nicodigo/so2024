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
    char letra;
    while(1){
        while(*dirContador == 0); //buffer vacio
        //seccion critica
        letra = dirbuffer[ind];
        *dirContador = *dirContador - 1;
        //fin seccion critica
        
        ind = (ind + 1) % 5;
        printf("%c\n", letra);
        if (letra == 'Z'){
            printf("Termina Productor\n");
            exit(0);
        }else{
            letra++;
        }


    }
}