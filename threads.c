#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

void *thread_routine(void *arg){
    int nr_lines = * ((int *)arg);
    int fd;
    char buf[] = "Nueva linea \n";

    printf("El hilo comienza a ejecutarse\n");

    for(int i = 0; i< nr_lines ; i++){
        fd = open("/home/cardan124/Documents/tmp.txt",O_WRONLY|O_APPEND);
        write(fd,buf,sizeof(buf)-1);
        close(fd);
    }

}

int main(int argc,char * argv[]){

 pthread_t thread1;
 int value = 0;
 if(argc>1)
    value = atoi(argv[1]);
 else
    printf("Pasa el argumento\n");

 if( 0 != pthread_create(&thread1,NULL,thread_routine,&value))
    return -1;

 pthread_join(thread1,NULL);
 
 return 0;
}