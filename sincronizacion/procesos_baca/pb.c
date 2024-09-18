#include <stdio.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <unistd.h>

#define Sa 0
#define Sb 1
#define Sc 2
#define Sx 3

void p(int semid, int semn);
void v(int semid, int semn);

int main(){
    int semid = semget(0xa, 0, 0);
    printf("semid = %d\n", semid);
    FILE * f ;
    int n = 10;
    while(n--){
        p(semid, Sb);
        p(semid, Sx);
        f = fopen("log.txt", "a");
        fprintf(f, "B ");
        
        sleep(1);
        fclose(f);
        v(semid, Sa);
        v(semid, Sc);
        printf("PB ...\n");
    }
    
}

void p(int semid, int semn){
    struct sembuf sb;

    sb.sem_num = semn;
    sb.sem_op = -1;
    sb.sem_flg = 0;
    semop(semid, &sb, 1);
}
void v(int semid, int semn){
    struct sembuf sb;

    sb.sem_num = semn;
    sb.sem_op = 1;
    sb.sem_flg = 0;
    semop(semid, &sb, 1);
}