#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_LINES 10




int main()
{
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Error en la creacion del socket");
        exit(1);
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        perror("Direccion invalida o no soportada");
        exit(1);
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("conexion fallida");
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

        int x = 0;
        int y = 1;
        while(1)
        {
            if (recv(sock, buffer, BUFFER_SIZE, 0) > 0)
            {
                printf("\033[s");
                fflush(stdout);
                printf("\033[%d;%dH", y, x); // Mover el cursor a la posición anterior
                fflush(stdout);
                
                
                printf("\r\033[K\r");
                printf("%s", buffer);
                fflush(stdout);
                y += 1;
                x = 0;
                if (y == MAX_LINES) y = 1;
                
                        
                

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
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        printf("\n\n\n\n%s\n", buffer);
        int result = -2;
        if ((result=send(sock, buffer, BUFFER_SIZE, 0)) > 0)
            printf("\n\n Mensaje enviado con exito!\n");
        else
            perror("\n\n Error al enviar el mensaje\n");
        
        printf("%d\n", result);
    }
    wait(NULL);
    close(sock);
    exit(0);


}