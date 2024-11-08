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
#define MAX_CLIENTES 10


int main()
{
    int cant_clientes = 0;
    int fd_pipe_clientes[2], fd_pipe_mensajes[2];

    if (pipe(fd_pipe_clientes) == -1 || pipe(fd_pipe_mensajes) == -1)
    {
        perror("Error al crear el pipe");
        exit(1);
    }
    int server_fd, new_socket;
    int clientes[MAX_CLIENTES] = {0};
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

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
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
    int pid_hijo = -1;
    pid_hijo = fork();
    if (pid_hijo == 0)
    {
        close(fd_pipe_mensajes[0]);
        close(fd_pipe_mensajes[1]);
        close(fd_pipe_clientes[0]);
        printf("Esperando conexion\n");
        while ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) > 0)
        {
            while (cant_clientes >= MAX_CLIENTES);
            printf("Conexion establecida\n");
            close(fd_pipe_clientes[0]);
            close(fd_pipe_mensajes[0]);
            close(fd_pipe_mensajes[1]);
            write(fd_pipe_clientes[1], &new_socket, sizeof(int));
            cant_clientes++;
            pid_hijo = fork();
            printf("pid hijo = %d", pid_hijo);
            if (pid_hijo == 0)
            {
                close(fd_pipe_mensajes[0]);
                int valread;
                printf("esperando mensaje sock: %d\n", new_socket);
                while ((valread = recv(new_socket, buffer, BUFFER_SIZE, 0)) > 0)
                {
                    printf("Hijo %s\n", buffer);
                    write(fd_pipe_mensajes[1], &buffer, BUFFER_SIZE);

                }
                exit(0);
            }
            
        }
        for (int i = 0; i < cant_clientes; i ++)
        {
            wait(NULL);
        }
        exit(0);
    }

    
    close(fd_pipe_clientes[1]);
    close(fd_pipe_mensajes[1]);
    fcntl(fd_pipe_clientes[0], F_SETFL, O_NONBLOCK);
    fcntl(fd_pipe_mensajes[0], F_SETFL, O_NONBLOCK);
    while(1)
    {
        if (read(fd_pipe_mensajes[0], &buffer, BUFFER_SIZE) > 0)
        {
            printf("Padre %s\n",buffer);
            for (int i = 0; i < cant_clientes; i++)
            {
                send(clientes[i], buffer, BUFFER_SIZE, 0);
            }
        }

        if (cant_clientes < MAX_CLIENTES && (read(fd_pipe_clientes[0], &new_socket, sizeof(int)) > 0))
        {
            clientes[cant_clientes] = new_socket;
            cant_clientes++;
        }
    }
    for (int i = 0; i < cant_clientes; i++)
    {
        wait(NULL);
    }
    close(fd_pipe_clientes[1]);
    close(fd_pipe_mensajes[1]);
    close(server_fd);
    exit(0);


}