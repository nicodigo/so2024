#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define EOT 4 //End of transmission
#define BUF_SIZE 100


int agregar_caracter2(char buf[BUF_SIZE])
{
    char ultima_letra;
    for (int i = 0; i < BUF_SIZE; i++)
    {
        if(buf[i] == '\0')
        {
            if (ultima_letra == 'z')
            {
                buf[0]= EOT;
                return 0;
            }else
            {
                buf[i] = ultima_letra + 1;
                return 1;
            }
           
        }else if(buf[i] == EOT)
        {
            return 0;
        }

        ultima_letra = buf[i];
    }
    return 0;
}


int agregar_caracter(char buf[BUF_SIZE])
{
    char ultima_letra;
    for (int i = 0; i < BUF_SIZE; i++)
    {
        if(buf[i] == '\0')
        {
            buf[i] = ultima_letra + 1;
                break;
            
        }else if (buf[i] == 'z')
        {
            buf[0] = EOT;
            return 0;
        }
        ultima_letra = buf[i];
    }
    if (ultima_letra == 'y') {
        return 0;
    }
    
    return 1;
}



int main()
{
    int fd1[2], fd2[2], fd3[2]; //tuberias fd[0] leer; fd[1] escribir
    char buffer[BUF_SIZE] = {0};
    int run = 1;

    __pid_t pidB, pidC;
    if (pipe(fd1) == -1 || pipe(fd2) == -1 || pipe(fd3) == -1 )
    {
        printf("Error en la creacion de tuberias\n");
        exit(1);
    }

    pidB = fork();
    if (pidB < 0)
    {
        printf("Error al crear el proceso B\n");
        exit(1);
    } else if (pidB == 0)
    {
        printf("Soy el proceso hijo B, con el PID: %d\n", getpid());
        close(fd3[0]);
        close(fd3[1]);
        close(fd1[1]);
        close(fd2[0]);
        while (run)
        {
            read(fd1[0], buffer, sizeof(buffer));
            if (buffer[0] == EOT) break;
            printf("Proceso B(%d) leo: %s\n", getpid(), buffer);
            run = agregar_caracter(buffer);
            write(fd2[1], buffer, sizeof(buffer));
        }
        close(fd1[0]);
        close(fd2[1]);
        printf("Termina proceso B\n");
        exit(0);
    }


    pidC = fork();
    if (pidC < 0)
    {
        printf("Error al crear el proceso C\n");
        exit(1);
    } else if (pidC == 0)
    {
        printf("Soy el proceso hijo c, con el PID: %d\n", getpid());
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[1]);
        close(fd3[0]);
        while (run)
        {
            read(fd2[0], buffer, sizeof(buffer));
            if (buffer[0] == EOT) break;
            printf("Proceso C(%d) leo: %s\n", getpid(), buffer);
            run = agregar_caracter(buffer);
            write(fd3[1], buffer, sizeof(buffer));
        }
        close(fd2[0]);
        close(fd3[1]);
        printf("Termina proceso C\n");
        exit(0);
    }

    printf("Soy el proceso padre, con el PID: %d... Cree los procesos B: %d   y   C: %d\n", getpid(), pidB, pidC);
    close(fd2[0]);
    close(fd2[1]);
    close(fd1[0]);
    close(fd3[1]);
    buffer[0] = 'a';
    buffer[1] = '\0';
    write(fd1[1], buffer, sizeof(buffer));
    
    while (run)
    {
        read(fd3[0], buffer, sizeof(buffer));
        if (buffer[0] == EOT) break;
        printf("Proceso A(%d) leo: %s\n", getpid(), buffer);
        run = agregar_caracter(buffer);
        write(fd1[1], buffer, sizeof(buffer));
       
    }


    close(fd1[1]);
    close(fd3[0]);

    wait(NULL);
    wait(NULL);
    printf("Termina el proceso A\n");

    exit(0);

}



