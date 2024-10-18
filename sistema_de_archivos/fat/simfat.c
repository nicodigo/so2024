#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 10 // tamaño del bloque
#define DISK_SIZE 100 // cantidad de bloques que tiene el disco
#define MAX_ARCHIVOS 100 // cantidad maxima de archivos por directorio

//#define EOF -1
#define NULO -2


typedef struct {
    char nombre[32];
    int size;
    int bloque_comienzo;
}Archivo;

int fat[DISK_SIZE]; //tabla fat
int espacio_disponible;// espacio disponible en el disco
Archivo directorio[MAX_ARCHIVOS];

void inicializar_fat()
{
    for (int i = 0; i < DISK_SIZE; i++)
    {
        fat[i] = NULO;
        directorio[i].bloque_comienzo = NULO;
    }
    espacio_disponible = DISK_SIZE;
}

int get_primer_bloque_disponible()
{
    for (int i = 0; i < DISK_SIZE; i++)
    {
        if (fat[i] == NULO){
            return i;
        }
    }
    return NULO;
}

int archivo_existe(char * nombre)
{
    for (int i = 0; i < MAX_ARCHIVOS; i++)
    {
        if (strcmp(nombre, directorio[i].nombre) == 0)
        {
            return 1;
        }
    }
    return 0;
}

void crear_archivo(char *nombre, int size)
{
    int bloques_necesarios = ((size + BLOCK_SIZE - 1 )  / BLOCK_SIZE);
    printf("Bloques_necesarios : %d\n", bloques_necesarios);
    if (bloques_necesarios > espacio_disponible)
    {
        printf("Error: No hay espacio disponible en el disco\n");
        return;
    }

    

    if (archivo_existe(nombre)){
        printf("Error: El nombre del archivo ya existe\n");
        return;
    }

    int ind_dir = -1;
    for (int i = 0; i < MAX_ARCHIVOS; i++)
    {
        if (directorio[i].bloque_comienzo == NULO){
            ind_dir = i;
            break;
        }    
    }
    if (ind_dir == -1)
    {
        printf("Error: No hay espacio en el directorio\n");
        return;
    }
    
    Archivo a;
    a.bloque_comienzo = get_primer_bloque_disponible();
    strcpy(a.nombre, nombre);
    a.size = size;

    

    directorio[ind_dir] = a;
    int bloque_anterior = a.bloque_comienzo;
    

    for (int i = bloque_anterior; i < DISK_SIZE && bloques_necesarios > 0; i++)
    {
        if (fat[i] == NULO)
        {
            fat[bloque_anterior] = i;
            fat[i] = EOF;
            bloque_anterior = i;
            espacio_disponible--;
            bloques_necesarios--;
        }
    }

    printf("Archivo creado \n");

}

void eliminar_archivo(char * nombre)
{
    int ind_dir = -1;
    for (int i = 0; i < MAX_ARCHIVOS; i++)
    {
        if (strcmp(directorio[i].nombre, nombre) == 0)
        {
            ind_dir = i;
        }
    }

    if (ind_dir == -1)
    {
        printf("Error: El archivo no existe\n");
        return;
    }

    int bloque_actual = directorio[ind_dir].bloque_comienzo;
    int prox_bloque;
    while (fat[bloque_actual] != EOF)
    {
        prox_bloque = fat[bloque_actual];
        fat[bloque_actual] = NULO;
        bloque_actual = prox_bloque;
    }
    fat[bloque_actual] = NULO;
    espacio_disponible += directorio[ind_dir].size;
    directorio[ind_dir].bloque_comienzo = NULO;
    

}

void mostrar_fat()
{
    printf("-----------------------\n");
    printf("|         FAT         |\n");
    printf("-----------------------\n");
    
    printf("|%-10s|%-10s|\n","BLOQUE","BLOQUE SIG");
    for(int i = 0; i < DISK_SIZE; i ++)
    {

        switch (fat[i])
        {
        case EOF:
            printf("|%-10d|%-10s|\n",i,"EOF");
            break;
        
        case NULO:
            printf("|%-10d|%-10s|\n",i,"VACIO");
            break;
        
        default:
            printf("|%-10d|%-10d|\n",i,fat[i]);
            break;
        }
        
    }
    printf("-----------------------\n");

}

void mostrar_directorio()
{
    printf("---------ARCHIVOS--------\n");
    
    printf("%-32s%-10s%-10s\n","NOMBRE","BLOQ INI","TAMANIO");
    for(int i = 0; i < DISK_SIZE; i ++)
    {
        printf("%-32s%-10d%-10d\n",directorio[i].nombre, directorio[i].bloque_comienzo, directorio[i].size);
    }

}

void print_menu_princ()
{
    printf("-----     Menu Principal     -----\n");
    printf("--  1. Ver FAT\n");
    printf("--  2. Ver Directorio\n");
    printf("--  3. Crear Archivo\n");
    printf("--  4. Eliminar Archivo\n");
    printf("--  0. Salir\n");
    printf("----------------------------------\n");
}

char * get_nombre()
{
    char *nombre = malloc(sizeof(char) * 32);
    if (nombre == NULL) return nombre;
    printf("Ingrese el nombre \n");
    printf("> ");
    while(fgets(nombre,32,stdin) == NULL)
    {
        char c;
        while ((c = getchar()) != '\n' && c != EOF);
        printf("Error: ingrese el nombre nuevamente\n");
        printf("> ");
    }
    char * aux = nombre;
    while(*aux != '\n' && *aux != '\0')
        aux++;
    *aux = '\0';
    return nombre;

}



int main()
{
    inicializar_fat();
    int opcion;
    int running = 1;
    while(running){
        print_menu_princ();
        printf("> ");
        while(scanf("%d",&opcion) != 1)
        {
            char c;
            while ((c = getchar()) != '\n' && c != EOF);
            
            printf("Error: Ingrese un numero entero\n");
            printf("> ");
            
        }
        getchar();
        switch (opcion)
        {
        case 1:
        {
            mostrar_fat();
        }break;
        
        case 2:
        {
            mostrar_directorio();
        }break;

        case 3:
        {
            char * nombre = get_nombre();
            int size;
            printf("Ingrese el tamanio: \n");
            printf("> ");
            while(scanf("%d",&size) != 1)
            {  
                char c;
                while ((c = getchar()) != '\n' && c != EOF);
                printf("Error: Ingrese un numero entero\n");
                printf("> ");
            }
            getchar();
            crear_archivo(nombre, size);

        }break;

        case 4:
        {
            char * nombre = get_nombre();
            eliminar_archivo(nombre);
        }break;

        case 0:
        {
            running = 0;
        }break;
        
        default:
        {
            printf("Opcion no valida\n");
        }break;
        }

    }
    
        
}