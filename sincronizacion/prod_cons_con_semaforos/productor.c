#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <string.h>
#include "semOps.h"

char * producir[] = { "Hola ", "como ", "estás?\n", "espero ", "que ", "bien, ", "este ", "es ",
                        "un ", "programa ", "para ", "jugar ", "con ", "semaforos, ", "te mando ",
                        "un ", "saludo.\n", "chau"}; 

int main(){
    
    int shmContador = shmget(0xa, 0, 0);
    int shmBuffer = shmget(0xb, 0, 0);

    int semid = semget(0xa,0,0);
    
    printf("ContadorID: %d\n", shmContador);
    printf("BufferID: %d\n", shmBuffer);
    printf("SemID: %d\n", semid);      
   
    int * dirContador = (int *) shmat(shmContador, NULL, 0);
    char * dirBuffer = (char *) shmat(shmBuffer,NULL,0);

    int ind = 0;
    int pos = 0;
    while (1){
        while((BUFFERSIZE - *dirContador) < strlen(producir[ind]));
        p(semid, 0);
        strcpy(dirBuffer, producir[ind]);
        *dirContador = *dirContador + strlen(producir[ind]);
        v(semid, 0);
        printf("%s", producir[ind]);
        fflush(stdout);
        sleep(1);
        
        if (strcmp(producir[ind++], "chau") == 0) break;
    }

    
    printf("Contador = %d", *dirContador);
    printf("fin productor\n");
}