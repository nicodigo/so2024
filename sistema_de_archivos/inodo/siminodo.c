#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLOCK_SIZE 25 // tamaño del bloque
#define DISK_SIZE 50 // cantidad de bloques que tiene el disco
#define MAX_ARCHIVOS  5// cantidad maxima de archivos por directorio
#define BLOQUES_DIRECTOS 7 // cantidad de punteros a bloques de datos en un inodo

//#define EOF -1
#define NULO -2

typedef struct
{
    char nombre[32];
    int size;
    int directo[BLOQUES_DIRECTOS]; //puteros a bloques de datos
    int indirecto_simple; //puntero al bloque indice
    int en_uso;

} inodo;

int espacio_disponible = DISK_SIZE;
char disco[DISK_SIZE][BLOCK_SIZE] = {0};
inodo directorio[MAX_ARCHIVOS] = {0};
int mapa_bloques[DISK_SIZE] = {0}; //Mapa de bloques libres mapa[i] = 0|1 ------> 0 => bloque[i] libre | 1 => bloque[i] ocupado

int archivo_existe(char * nombre)
{
    for (int i = 0; i < MAX_ARCHIVOS; i++)
    {
        if (directorio[i].en_uso && strcmp(nombre, directorio[i].nombre) == 0)
        {
            return 1;
        }
    }
    return 0;
}

int indice_inodo_libre()
{
    
    for (int i = 0; i < MAX_ARCHIVOS; i++)
    {
        if (directorio[i].en_uso == 0){
            return i;
        }    
    }
    return -1;
}

int get_bloques_libres(int size, int bloques[])
{
    int count_libres = 0;
    
    for (int i = 0; count_libres < size && i < DISK_SIZE; i++)
    {
        if (mapa_bloques[i] == 0)
        {
            bloques[count_libres] = i;
            count_libres++;
        }
        
    }
    return count_libres;
}

void crear_archivo(char * nombre, int size)
{
    if (size <= 0) return;
    size++;//espacio para el EOF
    int bloques_necesarios = ((size + BLOCK_SIZE - 1 )  / BLOCK_SIZE);
    printf("Bloques_necesarios : %d\n", bloques_necesarios);
    if (bloques_necesarios > BLOQUES_DIRECTOS) bloques_necesarios++;
    if (bloques_necesarios > espacio_disponible)
    {
        printf("Error: No hay espacio disponible en el disco\n");
        return;
    }

    if (archivo_existe(nombre)){
        printf("Error: El nombre del archivo ya existe\n");
        return;
    }

    int ind_dir = indice_inodo_libre();
    if (ind_dir == -1)
    {
        printf("Error: No hay espacio en el directorio\n");
        return;
    }
    
    strcpy(directorio[ind_dir].nombre, nombre);
    directorio[ind_dir].size = size;
    directorio[ind_dir].indirecto_simple = EOF;
    directorio[ind_dir].en_uso = 1;
    int libres[BLOQUES_DIRECTOS];
    int bloques_asignados = get_bloques_libres(BLOQUES_DIRECTOS, libres);
    for (int i = 0; i < bloques_asignados && bloques_necesarios > 0; i++){
        int bloque_actual = libres[i];
        directorio[ind_dir].directo[i] = libres[i];
        char letra = directorio[ind_dir].nombre[0]; 
        mapa_bloques[bloque_actual] = 1;
        bloques_necesarios--;
        espacio_disponible--;
        printf("BN: %d\n", bloques_necesarios);
        for (int j = 0; j < BLOCK_SIZE && size > 0; j++)
        {
            if (size == 1)
            {
                disco[bloque_actual][j] = EOF; 
            }else
            {
                disco[bloque_actual][j] = letra;
            }
            size--;
            
        }
        
    }

    
   
    
    if (bloques_necesarios > 0)
    {
        int libres[bloques_necesarios];
        printf("BN: %d\n", bloques_necesarios);
        bloques_asignados = get_bloques_libres(bloques_necesarios, libres);
        printf("BaN: %d\n", bloques_asignados);
        printf("a\n");
        directorio[ind_dir].indirecto_simple = libres[0];
        printf("a\n");
        printf("%d\n", libres[0]);
        mapa_bloques[libres[0]] = 1;
        printf("a\n");
        bloques_necesarios--;
        espacio_disponible--;
        printf("a\n");
        for (int i = 1, ind_bloq = 0; i < bloques_asignados && bloques_necesarios > 0 && ind_bloq <= BLOCK_SIZE; i++, ind_bloq++)
        {
            int bloque_indirecto = libres[0];
            int bloque_actual = libres[i];
            char letra = directorio[ind_dir].nombre[0]; 

            disco[bloque_indirecto][ind_bloq] = bloque_actual;
            mapa_bloques[bloque_actual] = 1;
            bloques_necesarios--;
            espacio_disponible--;
            printf("a");
            for (int j = 0; j < BLOCK_SIZE && size > 0; j++)
            {
                if (size == 1)
                {
                    disco[bloque_actual][j] = EOF; 
                }else
                {
                    disco[bloque_actual][j] = letra;
                }
                size--;
            }
            printf("b");
        
        }
        
    }

    if (size == 0 && bloques_necesarios == 0 && espacio_disponible >= 0)
    {
        printf("Archivo creado con exito!\n");
    }else
    {
        printf("Algo salio mal :(\n");
        printf("Size: %d", size);
        printf("Bloq_necesaros: %d", bloques_necesarios);
        printf("Espacio_disp: %d", espacio_disponible);
    }
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

    int bloques_ocupados = ((directorio[ind_dir].size + BLOCK_SIZE - 1 )  / BLOCK_SIZE);

    for (int i = 0;  i < BLOQUES_DIRECTOS && bloques_ocupados > 0; i ++)
    {
        mapa_bloques[directorio[ind_dir].directo[i]] = 0;
        directorio[ind_dir].directo[i] = EOF;
        bloques_ocupados--;
        espacio_disponible++;
    }

    if (bloques_ocupados > 0)
    {
        int bloque_indice = directorio[ind_dir].indirecto_simple;
        for (int i = 0; bloques_ocupados > 0 && i < BLOCK_SIZE; i++)
        {
            mapa_bloques[disco[bloque_indice][i]] = 0;
            bloques_ocupados--;
            espacio_disponible++;
        }
    }
    mapa_bloques[directorio[ind_dir].indirecto_simple] = 0;
    directorio[ind_dir].en_uso = 0;
    directorio[ind_dir].indirecto_simple = EOF;
    

    if (bloques_ocupados == 0)
    {
        printf("Archivo eliminado con exito!\n");
    }
}







void print_menu_princ()
{
    printf("-----     Menu Principal     -----\n");
    printf("--  1. Ver Disco\n");
    printf("--  2. Ver Directorio\n");
    printf("--  3. Ver Mapa de Bloques Libres\n");
    printf("--  4. Crear Archivo\n");
    printf("--  5. Eliminar Archivo\n");
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

void mostrar_disco()
{
    
    printf("     ");
    for (int i = 0; i < BLOCK_SIZE; i++)
    {
        printf("%3d ",i);
    }
    printf("\n");
    for (int i = 0; i < DISK_SIZE; i++)
    {
        printf("%-4d ",i);
        for (int j = 0; j < BLOCK_SIZE; j++)
        {
            printf("%3d ", disco[i][j]);
        }
        printf("\n");
    }
}

void inicializar_inodos()
{
    for (int i = 0; i < MAX_ARCHIVOS; i++)
    {
        for (int j = 0; j < BLOQUES_DIRECTOS; j++)
        {
            directorio[i].directo[j] = EOF;
        }
    }
}

void mostrar_directorio()
{
    printf("---------INODOS--------\n");
    
    for (int i = 0; i < MAX_ARCHIVOS; i++)
    {
        if (directorio[i].en_uso == 1)
        {
            printf("----------------------\n");
            printf("Nombre: %s\n", directorio[i].nombre);
            printf("Tamanio: %d\n", directorio[i].size);
            printf("Bloques Directos: \n");
            for (int j = 0; j < BLOQUES_DIRECTOS; j++)
                printf("%d: %d\n", j, directorio[i].directo[j]);
            printf("Indirecto simple: %d\n", directorio[i].indirecto_simple);
            printf("----------------------\n");
        }
    }
    printf("-------------------------------\n");

}

void mostrar_mapa_bloques()
{
    for (int i = 0; i < DISK_SIZE; i++)
    {
        printf("%d: ", i);
        if (mapa_bloques[i])
        {
            printf(" Ocupado\n");
        }else
        {
            printf(" Vacio\n");
        }
    }
}

int main()
{
    int opcion;
    int running = 1;
    inicializar_inodos();
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
            mostrar_disco();
        }break;
        
        case 2:
        {
            mostrar_directorio();
        }break;

        case 3:
        {
            mostrar_mapa_bloques();
        }break;

        case 4:
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

        case 5:
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