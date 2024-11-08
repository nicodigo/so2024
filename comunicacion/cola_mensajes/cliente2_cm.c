#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define BUFFER_SIZE 512
#define MAX_LINES 10


typedef struct Posicion
{
    int x;
    int y;
}Posicion;

typedef struct Mensaje
{
    long tipo;
    char texto[BUFFER_SIZE];
}Mensaje;

int main()
{
    int mi_id_msg = 2;
    Mensaje msj;
    int clave = 0x0a;

    int msgqid = msgget(clave, 0666|IPC_CREAT);
    if (msgqid == -1)
    {
        perror("Error al crear la cola de mensajes");
        exit(1);
    }

    msj.tipo = mi_id_msg;
    printf("\033[H\033[J");
    fflush(stdout);

    pid_t pidH = fork();
    
    if (pidH < 0)
    {
        perror("Error al crear el proceso hijo");
        exit(1);
    }else if (pidH == 0)
    {
        sleep(2);

        
        Posicion posAnterior;
        posAnterior.x = 0;
        posAnterior.y = 1;
        while(1)
        {
            
            if (msgrcv(msgqid, &msj, sizeof(msj), 1, 0) > 0)
            {
                printf("\033[s");
                fflush(stdout);
                printf("\033[%d;%dH", posAnterior.y, posAnterior.x); // Mover el cursor a la posición anterior
                fflush(stdout);
                
                
                printf("\r\033[K\r");
                printf("Cliente 1: %s", msj.texto);
                
                fflush(stdout);
                posAnterior.y += 1;
                posAnterior.x = 0;
                if (posAnterior.y == MAX_LINES) posAnterior.y = 1;
                
                        
                

                printf("\033[u"); // Mover el cursor a la posición de escritura
                fflush(stdout);
            }
            
        }
        exit(0);
    }
    while (1)
    {
        
        printf("\033[%d;0H", MAX_LINES+1); // Mover el cursor a la posición de escritura
        printf("\r\033[K\r> ");
        fflush(stdout);
        fgets(msj.texto, BUFFER_SIZE, stdin);
        msj.texto[strcspn(msj.texto, "\n")] = 0;
        if (strcmp(msj.texto, "q!") == 0)
        {
            kill(pidH, SIGTERM);
            break;
        }
        int result = -2;
        if ((result=msgsnd(msgqid, &msj, sizeof(msj), 0)) == 0)
            printf("\n\n Mensaje enviado con exito!\n");
        else
            perror("\n\n Error al enviar el mensaje\n");
        
        printf("%d\n", result);
    }
    wait(NULL);
    
    exit(0);
}