//ALGORITMO DE DEKKER 5
//USO DE LA VARIABLE TURNO 
// ALGORITMO PARA LA EXCLUSION MUTUA
//PRIMERA SOLUCION CREADA TOTALMENTE EN SOFTWARE
//USA FLAGS PARA DETERMINAR CUAL PROCESO QUIERE ENTRAR A LA SECCION CRITICA
//INCORPORA UNA NUEVA FORMA PARA IDENTIFICAR CUAL DE LSO PROCESOS TIENE PRIORIDAD
//EXLUSION MUTUA SOLO PARA DOS PROCESOS
//INCORPORA PRIORIDAD EN LOS PROCESOS

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<wait.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>

//INICIO REGION CRITICA (CONTEO 0-2)
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
//INICIO REGION NO CRITICA DEL HIJO (CONTEO 0-19)
void regionNoCriticaProcesoHijo(int PID)
{
   int i;
   printf("\nHijo en la region NO critica proceso con pid=%d\n",PID);
   for(i=0;i<20;i++)
   {
      printf("\nHijo: %d\n",i);
      sleep(3);
   }
}
//INICIO REGION NO CRITICA DEL PADRE (CONTEO 0-2)
void regionNoCriticaProcesoPadre(int PID)
{
   int i;
   printf("\nPadre en la region NO CRITICA proceso con pid=%d\n",PID);
   for(i=0;i<3;i++)
   {
      printf("\nPadre: %d\n",i);
      sleep(3);
   }
}

int main()
{
   pid_t pid;
   int PID;
   int shmid1;
   int shmid2;
   int shmid3;
   int i;
   int *Hijo_desea_entrar;
   int *Padre_desea_entrar;
   int *Proceso_favorecido;
   key_t llave1;
   key_t llave2;
   key_t llave3;
   llave1=ftok("Prueba1",'k');
   llave2=ftok("Prueba2",'l');
   llave3=ftok("Prueba3",'m');
   shmid1=shmget(llave1,sizeof(int),IPC_CREAT|0600);
   shmid2=shmget(llave2,sizeof(int),IPC_CREAT|0600);
   shmid3=shmget(llave3,sizeof(int),IPC_CREAT|0600);
   Hijo_desea_entrar=shmat(shmid1,0,0);
   Padre_desea_entrar=shmat(shmid2,0,0);
   Proceso_favorecido=shmat(shmid3,0,0);
   *Hijo_desea_entrar=1; //HIJO FLAG
   *Padre_desea_entrar=0; //PADRE FLAG
   *Proceso_favorecido=1; //QUIEN TIENEN LA PRIODIDAD (HIJO)
   pid= fork(); //CREAMOS EL PROCESO
   if(pid==-1)
   {
      perror("\nError al crear el proceso\n");
      exit(-1);
   }

//PUEDEN LSO DOS PROCESOS QUERER ENTRAR AL MISMO TIEMPO, PERO EL QUE TIENE LA PRIORIDAD
//INICIAL ES EL PROCESO HIJO.

   if(pid==0) // PROCESO HIJO
   {
      while (1) //CICLO INFINITO
      {
         *Hijo_desea_entrar=1; //DESEA ENTRAR, VERDADERO, SE VA A SECCION CRITICA
         while(*Padre_desea_entrar) //PADRE E HIJO DESEAN ENTRAR A LA S.C (VER LINEA ANTERIOR)
         {
            if(*Proceso_favorecido==2) // EN EL PRIMER CASO EL QUE TIENE LA PRIORIDAD ES EL HIJO, POR LO QUE ESTO LO IGNORA Y VA A LA S.C
            {                          //EN EL SEGUNDO CASO EL PADRE SI TIENEN LA PRIORIDAD Y ENTRA AL IF
               *Hijo_desea_entrar=0;   // PONEN EN ESPERA AL HIJO 
               while(*Proceso_favorecido==2); //SI EL PROCESO FAVORECIDO ES EL PADRE ENTONCES LOOP PARA ESPERAR
               *Hijo_desea_entrar=1;            //DESPUES CEDE LA BANDERA DE ENTRADA A LA S.C AL HIJO
            }
         }
         PID=getpid();
         regionCritica(PID); // TOMA LA SECCION CRITICA AL SALTARSE EL WHILE DEL PADRE
         *Proceso_favorecido=2; //CEDE LA PRIORIDAD AL PROCESO PADRE
         *Hijo_desea_entrar=0; //DECLINA SU ENTRADA A LA SECCION CRITICA PORQUE YA TERMINO
         regionNoCriticaProcesoHijo(PID);
      }
   }
   else
   {
      while(1) // PROCESO PADRE
      {
         *Padre_desea_entrar=1;
         while(*Hijo_desea_entrar)
         {
            if(*Proceso_favorecido==1)
            {
               *Padre_desea_entrar=0;
               while(*Proceso_favorecido==1);
               *Padre_desea_entrar=1;
            }
         }
         PID=getpid();
         regionCritica(PID);
         *Proceso_favorecido=1;
         *Padre_desea_entrar=0;
         regionNoCriticaProcesoPadre(PID);
      }
   }
   return 0;
}