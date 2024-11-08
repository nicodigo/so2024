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

int main()
{
    int server_fd, new_socket;
    
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};


    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("Error al crear el socket");
        exit(1);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    memset(address.sin_zero, 0, 8);

    if (bind(server_fd, (struct sockaddr *)&address, addrlen) < 0)
    {
        perror("Error en el bind");
        close(server_fd);
        exit(1);
    }

    if (listen(server_fd, 3) < 0)
    {
        perror("Error en el listen");
        close(server_fd);
        exit(1);
    }

    printf("Servidor creado con exito, escuchando en el puerto: %d\n", PORT);

    while((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)))
    {
        printf("conexion establecida \n");
        while (1)
        {
            recv(new_socket, buffer, BUFFER_SIZE, 0);
            printf("El cliente %d dice: %s\n", new_socket, buffer);
            if (strcmp(buffer, "q!") == 0) break;
            memset(buffer, 0, BUFFER_SIZE);
            
            printf("Ingrese un mensaje para el cliente: ");
            fgets(buffer, BUFFER_SIZE, stdin);
            buffer[strcspn(buffer, "\n")] = 0;
            send(new_socket, buffer, BUFFER_SIZE, 0);

        }
    }
    close (server_fd);
    exit(0);
}