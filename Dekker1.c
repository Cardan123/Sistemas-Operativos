//Algoritmo de Dekker imtento 1 ALTERNANCIA ESTRICTA
//Algoritmo concurrente para la exclusiòn mutua
//Ayuda a que dos procesos compartan un recurso sin tener problemas
//La principal desventaja es que los procesos lentos atrasan a los procesos 
//rapidos.
//Dos procesos quieren utilizar un recurso, cuando el proceso 1 lo utiliza
// da la señal de que el lo esta utilizando, y el proceso dos espera y checa hasta 
//que se desocupa la secciòn critica y entra y pone la señal de que el lo esta utilizando ahora

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
//INICIO REGIÒN CRITICA
void regionCritica(int PID)
{
   int i;
   printf("\nProceso en la region critica proceso con pid=%d\n",PID);
   for(i=0;i<3;i++)
   {
      printf("\nRegión critica: %d\n",i);
      sleep(3);	
   }
}
//FIN REGIÒN CRITICA
//INICIO REGIÒN NO CRITICA HIJO
void regionNoCriticaProcesoHijo(int PID)
{
   int i;
   printf("\nHijo en la region NO critica proceso con pid=%d\n",PID);
   for(i=0;i<25;i++)
   {
      printf("\nHijo: %d\n",i);
      sleep(3);
   }
}
//FIN REGIÒN NO CRITICA HIJO

//INICIO REGIÒN NO CRITICA PADRE
void regionNoCriticaProcesoPadre(int PID)
{
   int i;
   printf("\nPadre en la region NO critica proceso con pid=%d\n",PID);
   for(i=0;i<5;i++)
   {
      printf("\nPadre: %d\n",i);
      sleep(3);
   }
}
//INICIO REGIÒN NO CRITICA PADRE

//INICIO PROGRAMA PRINCIPAL
int main()
{
   pid_t pid; //PROCESS ID
   int PID;   // PROCESS ID
   int shmid;  //MEM. COMPARTIDA
   int i;
   int *turno; //MEM. COMPARTIDA
   key_t llave; //MEM. COMPARTIDA
   llave=ftok("Prueba",'k'); //MEM. COMPARTIDA
   shmid=shmget(llave,sizeof(int),IPC_CREAT|0600); //MEM. COMPARTIDA
   turno=shmat(shmid,0,0); //MEM. COMPARTIDA
   *turno=0; //INICIO A FAVOR DEL HIJO
   pid= fork(); // CREACION DEL PROCESO HIJO
   if(pid==-1) //  ERROR
   {
      perror("\nError al crear el proceso\n");
      exit(-1);

      
   }
   if(pid==0) // HIJO
   {
      while (1) //CICLO INFINITO, AQUI ENTRA SIN NINGUNA RESTRICCIÒN
      {

         PID=getpid(); 
         regionCritica(PID);
         *turno=1;  // AQUI CAMBIA TURNO A 1 PARA TOMAR EL TURNO Y PROVOCAR RETARDO EN EL PADRE
         regionNoCriticaProcesoHijo(PID);
      }
   }
   else // PADRE
   {
      while(1) //CICLO INFINITO
      {
         while (*turno!=1); //RETARDO DESDE EL PRIMER CICLO,TURNO ES 0
         PID=getpid();
         regionCritica(PID);
         *turno=0;   //INTENTA SEGUIR TENIENDO EL TURNO PARA EL MISMO
         regionNoCriticaProcesoPadre(PID);
      }
   }
   return 0;
}
//FIN PROG PRINCIPAL
