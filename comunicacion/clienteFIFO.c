#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "serv_fifo.h"

#define MAX_LINES 10

typedef struct 
{
    int x;
    int y;
} Posicion;

//procedimiento para conectarse al servidor
//ES BLOQUEANTE, por lo tanto el procedimiento se quedará esperando la respuesta del servidor
char * conectar(char * serv_fifo, pid_t pid, char *nombre, char * miFifo)
{

    char *server_path = malloc(BUFFER_SIZE);
    if (server_path == NULL)
    {
        printf("Error de malloc\n");
        exit(1);
    }
    Cliente c;
    c.pid = pid;
    strcpy(c.nombre, nombre);
    strcpy(c.fifoC, miFifo);

    int fd_servidor = open(serv_fifo, O_WRONLY);
    write(fd_servidor, &c, sizeof(c));

    int fd_miFifo = open(miFifo, O_RDONLY);
    read(fd_miFifo, server_path, BUFFER_SIZE);
    return server_path;
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Uso correcto: ./nombre_de_ejecutable \'/ruta_fifo_servidor_conexiones' \'nombre_del_cliente\'\n");
        printf("Ejemplo: /clienteFIFO.o \'/tmp/s1_fifo_conexiones' \'Cliente_1\'\n ");
        exit(0);
    }

    char  mi_fifo[BUFFER_SIZE] = sprintf(mi_fifo, "/tmp/fifo_cliente%d", getpid()); 
    char * fifo_conex_serv = argv[1];
    char * nombre = argv[2];

    

    int mi_fifo_fd;
    mkfifo(mi_fifo, 0666);
    mi_fifo_fd = open(mi_fifo, O_RDONLY|O_NONBLOCK);
    if (mi_fifo_fd < 0)
    {
        printf("Error al crear el fifo\n");
        exit(1);
    }

    int fifo_conex_fd;
    fifo_conex_fd = open(fifo_conex_serv, O_WRONLY|O_NONBLOCK);
    if (fifo_conex_fd < 0)
    {
        printf("Error al abrir el fifo de conexiones\n");
        exit(1);
    }

    char * fifo_mensajes = conectar(fifo_conex_serv, getpid(), nombre, mi_fifo);
    Cliente mi_cliente;
    strcpy(mi_cliente.nombre, nombre);
    mi_cliente.pid = getpid();
    strcpy(mi_cliente.fifoC, mi_fifo);
    
    int fifo_sv_msg_fd;
    fifo_sv_msg_fd = open(fifo_mensajes, O_WRONLY|O_NONBLOCK);
    if (fifo_sv_msg_fd < 0)
    {
        printf("Error al abrir el fifo de mensajes\n");
        exit(1);
    }


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

        Mensaje msg;
        Posicion posAnterior;
        posAnterior.x = 0;
        posAnterior.y = 1;
        while(1)
        {
            if (read(mi_fifo_fd, &msg, sizeof(msg)) > 0)
            {
                printf("\033[s");
                fflush(stdout);
                printf("\033[%d;%dH", posAnterior.y, posAnterior.x); // Mover el cursor a la posición anterior
                fflush(stdout);
                
                
                printf("\r\033[K\r");
                if (msg.c.pid != mi_cliente.pid) printf("%s(%d): %s", msg.c.nombre, msg.c.pid, msg.msg);
                else printf("yo(%d): %s", msg.c.pid, msg.msg);
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
        Mensaje msg;
        printf("\033[%d;0H", MAX_LINES+1); // Mover el cursor a la posición de escritura
        printf("\r\033[K\r> ");
        fflush(stdout);
        fgets(msg.msg, BUFFER_SIZE, stdin);
        msg.msg[strcspn(msg.msg, "\n")] = 0;
        printf("\n\n\n\n%s\n", msg.msg);
        printf("%s\n", fifo_mensajes);
        msg.c.pid = mi_cliente.pid;
        strcpy(msg.c.nombre, mi_cliente.nombre);
        strcpy(msg.c.fifoC, mi_cliente.fifoC);
        int result = -2;
        if ((result=write(fifo_sv_msg_fd, &msg, sizeof(msg))) > 0)
            printf("\n\n Mensaje enviado con exito!\n");
        else
            perror("\n\n Error al enviar el mensaje\n");
        
        printf("%d\n", result);
    }
    wait(NULL);
    close(fifo_conex_fd);
    close(fifo_sv_msg_fd);
    close(mi_fifo_fd);
    exit(0);

}