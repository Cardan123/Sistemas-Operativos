#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc,char *argv[]){
    int shmid, *variable;
    key_t llave;

    llave = ftok (argv[0], 'K' );

    if((shmid = shmget(llave,sizeof(int),IPC_CREAT | 0600)) == 1){
        perror("Error en shmget");
        exit(-1);
    }

    if((variable = (int *)shmat(shmid,NULL,0)) == (int *)(-1)){
        perror("Fallo shmat");
        exit(-1);
    }
    while(1){
        printf("\nIntroduzca m para modificar el valor de la variable, v para visualizarla y t para terminarla:\n");

        switch(leer_car()){
            case 't':
                shmctl(shmid,IPC_RMID,0);
                exit(0);
                break;
            case 'v':
                printf("variablle = %d\n",*variable);
                break;
            case 'm':
                printf("Nuevo valor de la variable en memoria compartida:\n");
                scanf("%d",variable);
                break;
            default:
                printf("Se introdujo una letra incorrecta = %d\n");
                break;
        }
    }
}

int leer_car(){
    char letra;
    char almacen[80];

    scanf("%s",&almacen);
    sscanf(almacen,"%c",&letra);
    return letra;
}
