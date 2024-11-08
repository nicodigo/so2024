#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define BUFFER_SIZE 512
#define MAX_CLIENTES 10

typedef struct Mensaje
{
    long tipo;
    char texto[BUFFER_SIZE];
}Mensaje;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Uso correcto: ./nombre_ejecutable \'clave_cola_mensajes\'\n");
        printf("Uso correcto: ./servidor_msgq \'1234\'\n");
        exit(0);
    }

    char * aux = argv[1];
    while (*aux)
    {
        if (*aux > '9' || *aux <'0')
        {
            printf("La clave debe ser un entero\n");
            exit(0);
        }
        aux++;
    }

    
    
    int cant_clientes = 0;
    int fd_conex, fd_msj;
    

}