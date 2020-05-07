#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/shm.h>

int main(int argc, char * argv[]){
    int i;
    int shmid, *variable;
    key_t llave;

    llave = ftok ("/tmp", 'K' );
    
    if((shmid=shmget(llave,sizeof(char), IPC_CREAT|0666))==-1){
        perror("shmget");
        exit(2);
    }
    
    if((variable = (int *) shmat(shmid,NULL,0)) == (int *) -1){
        perror("shmat");
        exit(2);
    }
    
    srand(getpid());
    
    
    for(i=0;i<10;i++)
        printf("%c ",variable[i]);
    
    putchar('\n');
}