#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "serv_fifo.h"


#define MAX_CLIENTES 10


int cant_conectados = 0;
int pipeInterno[2];
pid_t pid_hijos[MAX_CLIENTES];
Cliente clientes[MAX_CLIENTES];

void desconectar_cliente(int indice_c)
{
    for (int i = 0; i < cant_conectados-1; i++){
        clientes[i] = clientes[i+1]; 
    }
    cant_conectados--;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Uso correcto: ./nombre_de_ejecutable \'/ruta_fifo_conexiones\' \'/ruta_fifo_mensajes'\n");
        printf("Ejemplo: /servidorFIFO.o \'/tmp/s1_fifo_conexiones\' \'/tmp/s1_fifo_mensajes'\n ");
        exit(0);
    }
    char * fifo_conexiones = argv[1];
    char * fifo_mensajes = argv[2];
    mkfifo(fifo_mensajes, 0666);
    
    mkfifo(fifo_conexiones, 0666);
       
    int fd_fifo_mensajes, fd_fifo_conexiones;
    fd_fifo_conexiones = open(fifo_conexiones, O_RDONLY|O_NONBLOCK);
    
    if (fd_fifo_conexiones < 0)
    {
        printf("Error al abrir el archivo fifo de conexiones\n");
        exit(1);
    }
    
    fd_fifo_mensajes = open(fifo_mensajes, O_RDONLY|O_NONBLOCK);
    
    if (fd_fifo_mensajes < 0)
    {
        printf("Error al abrir el archivo fifo de mensajes\n");
        exit(1);
    }

    Cliente c;
    Mensaje m;
    while (1)
    {
        //si hay alguna conexion en espera, me fijo si tengo espacio para otro cliente
        //y si tengo espacio le asigno un proceso para que maneje el cliente
        if (cant_conectados < MAX_CLIENTES){
            if (read(fd_fifo_conexiones, &c, sizeof(c)) > 0)
            {
                int fd_respuesta = open(c.fifoC, O_WRONLY);
                if (fd_respuesta < 0)
                {
                    printf("Error al abrir el fifo del cliente %d\n", c.pid);
                 continue;
                }
            
                write(fd_respuesta, fifo_mensajes, BUFFER_SIZE);
                close(fd_respuesta);

                clientes[cant_conectados].pid = c.pid;
                strcpy(clientes[cant_conectados].nombre, c.nombre);
                strcpy(clientes[cant_conectados].fifoC, c.fifoC);
                cant_conectados++;
            }

            
            
        }

        if (read(fd_fifo_mensajes, &m, sizeof(m)) > 0)
        {
            for (int i = 0; i < cant_conectados; i++)
            {
                int fd_respuesta = open(clientes[i].fifoC, O_WRONLY|O_NONBLOCK);
                int est = write(fd_respuesta, &m, sizeof(m));
                if (est == -1) 
                {
                    desconectar_cliente(i);
                    i--;
                }
                close(fd_respuesta);
            }
        }

    }


}