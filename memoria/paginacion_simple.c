#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CANT_FRAMES 32 //Cantidad de particiones de la memoria fisica
//IMPORANTE: mantener siempre expresado en KB
#define PAG_SIZE 1 //Tamaño de las paginas en KB
#define MAX_PROC 10

#define NULO 0

typedef struct 
{
    int pid;
    int size; //tamanio en KB
    int cant_pags; //cantidad de paginas, se puede calcular sabiendo el tamaño del proceso pero lo incluyo por comodidad
    int *tabla_pag; 
}Proceso;


Proceso procesos[MAX_PROC];
int cant_procesos = 0;
int mapa_frames[CANT_FRAMES];
int cant_frames_libres = CANT_FRAMES;


int asignar_frames_libres(Proceso p)
{
    int count = 0;
    for (int i = 0; count < p.cant_pags && i < CANT_FRAMES; i++)
    {
        if (mapa_frames[i] == NULO)
        {
            p.tabla_pag[count] = i;
            mapa_frames[i] = p.pid;
            cant_frames_libres--;
            count++;
        }
    }
    return count;
}

void asignar_frames(Proceso p)
{
    if (p.cant_pags > cant_frames_libres)
    {
        printf("Error: no hay frames disponibles para alocar el proceso\n");
        return;
    }
    if (cant_procesos >= MAX_PROC)
    {
        printf("Error: se alcanzo el limite de procesos\n");
        return;
    }

    
    p.tabla_pag = (int *) malloc(sizeof(int) * p.cant_pags);
    if (p.tabla_pag == NULL)
    {
        return;
    } 

    int pags;
    if ((pags = asignar_frames_libres(p)) != p.cant_pags)
    {
        printf("%d\n", pags);
        printf("Error: hubo un error al asignar las paginas\n");
        for (int i = 0; i < pags; i++)
        {
            mapa_frames[p.tabla_pag[i]] = NULO;
            cant_frames_libres++;
        }
        return;
    }

    procesos[cant_procesos] = p;
    cant_procesos++;
    printf("Proceso %d creado exitosamente! \n", p.pid);
    
}


void crear_proceso()
{
    Proceso p;
    printf("Ingrese el PID: ");
    if (scanf("%d", &p.pid) != 1) return;
    printf("Ingrese el tamanio expreado en KB: ");
    if (scanf("%d", &p.size) != 1) return;
    p.cant_pags = ((p.size + PAG_SIZE - 1) / PAG_SIZE );
    
    asignar_frames(p);   
}

void eliminar_proceso(int pid)
{   
    int ind = -1;
    for (int i = 0; i < cant_procesos && ind == -1; i++)
    {
        if (pid == procesos[i].pid){
            printf("a\n");
            ind = i;
        }
    }

    if (ind == -1)
    {
        printf("Error: el PID no existe\n");
        return;
    }

    for (int i = 0; i < procesos[ind].cant_pags; i++)
    {
        mapa_frames[procesos[ind].tabla_pag[i]] = NULO;
        cant_frames_libres++;
    }

    for (int i = ind; i < cant_procesos; i++)
    {
        procesos[i] = procesos[i+1] ;
    }
    procesos[cant_procesos].pid = -1;
    cant_procesos--;
    printf("Proceso %d eliminado exitosamente!\n", pid);

}


int get_dir_fisica(int pid, int dir_logica)
{
    int ind = -1;
    for (int i = 0; i < cant_procesos && ind == -1; i++)
    {
        if (pid == procesos[i].pid)
        {
            ind = i;
        }
    }

    if (ind == -1)
    {
        printf("Error: el PID no existe\n");
        return -1;
    }

    Proceso p = procesos[ind];

    if ((dir_logica /(PAG_SIZE*1024)) > (p.cant_pags - 1))
    {
        printf("Error: Segmentation Fault\n");
        return -1;
    }

    int dir_fisica = p.tabla_pag[(dir_logica / (PAG_SIZE*1024))] * PAG_SIZE*1024 + (dir_logica % (PAG_SIZE*1024));

    return dir_fisica;
}


void mostrar_frames()
{
    printf("|-------|-------|\n");
    printf("| FRAME |B_VALID|\n");
    printf("|-------|-------|\n");
    for (int i = 0; i < CANT_FRAMES; i++)
    {
        printf("|  %-5x|  %-5d|\n", i, mapa_frames[i]);
    }
    
    printf("|-------|-------|\n");
}


void mostrar_procesos()
{
    for (int i = 0; i < cant_procesos; i++)
    {
        printf("PID: %d  |  Cant_pags: %d  \n", procesos[i].pid, procesos[i].cant_pags);
    }
}


void print_menu_principal()
{
    printf("\n-----          MENU PRINCIPAL          -----\n");
    printf("--             1. Nuevo proceso           --\n");
    printf("--             2. Terminar proceso        --\n");
    printf("--             3. Mostrar frames          --\n");
    printf("--             4. Mostrar procesos        --\n");
    printf("--             5. Buscar Direccion Fisica --\n");
    printf("--             0. Salir                   --\n");

}

int main()
{
    int opcion = -1;
    int run = 1;

    while(run)
    {
        print_menu_principal();
        while(scanf("%d", &opcion) != 1)
        {
            while (getchar() != '\n');            
        }

        switch (opcion)
        {
            case 1:
            {
                crear_proceso();
            }break;
            
            case 2:
            {
                int pid;
                printf("Ingrese el PID: ");
                if(scanf("%d", &pid) == 1){
                    eliminar_proceso(pid);
                }else
                {
                    while (getchar() != '\n');
                }
            }break;
            
            case 3:
            {
                mostrar_frames();
            }break;

            case 4:
            {
                mostrar_procesos();
            }break;

            case 5:
            {
                int pid;
                printf("Ingrese el PID: ");
                if(scanf("%d", &pid) == 1){
                    int dir_log;
                    printf("Ingrese la Direccion (hex): ");
                    if(scanf("%x", &dir_log) == 1){
                        int dir_fisica = get_dir_fisica(pid, dir_log);
                        if (dir_fisica != -1)
                        {
                            printf("Dir Logica: 0x%x %d(dec) ------> Dir Fisica: 0x%x %d(dec) \n", dir_log, dir_log, dir_fisica, dir_fisica);
                        }
                    
                    }else
                    {
                        while (getchar() != '\n');
                    }
                    
                }else
                {
                    while (getchar() != '\n');
                }
            }break;

            case 0:
                run = 0;
            break;

            default:
                printf("Opción no válida.\n");
        }
    }
}


