

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void pstreeSenal();
void lsSenal();
void alarmSenal();


int main(int argc, char *argv[])
{

signal(SIGUSR2, pstreeSenal);
signal(SIGUSR1, lsSenal);
signal(SIGALRM, alarmSenal); 
	
    if(argc == 3)
    {
		char nom[] = {'A','B','X','Y','Z'};
	  	pid_t pids[5];//Guardo ejec,A,B,X,Y(para las llamadas con exclp)
	  	pid_t pid;
	  	int i, tiempo,j;
	  	char letra = *argv[1];
	  	tiempo = atoi(argv[2]);
	    
	  	printf ("Soy el proceso ejec: mi pid es %d \n", getpid());
	  	pids[0] = getpid();
	  	pid = fork();
	  	if(pid > 0)//estamos en proceso arb y lo matamos
	  	{
			wait();
		  	printf("Soy ejec (%d) y muero.\n", pids[0]);
		  	exit(0);
	  	}
	  	if(pid == 0)//estamos en el hijo A
	  	{
			pid = fork();//hacemos otro hijo (B)
			pids[1]=getpid();
			if(pid > 0)//estamos en proceso A
			{
				printf("Soy el proceso A: mi pid es %d. Mi padre es %d \n", getpid(), getppid());
				wait();//espero a que muera B
				printf("Soy A (%d) y muero.\n", getpid());
				exit(0);
			}
			else if(pid==0)//estamos en B
			{
				printf ("Soy el proceso B: mi pid es %d. Mi padre es %d \n", getpid(),getppid());
		     	 /* Bucle de creación de hijos */
		    	for (j = 0; j < 3; j++)
		    	{
		    		pid=fork();//crear hijos
		      		pids[j+2]=getpid();
		      		if (pid == 0)//si estamos en el hijo muestra quien es y muere.
		      		{
						printf("Soy el proceso %c: mi pid es %d. Mi padre es %d. Mi abuelo es %d. Mi Bisabuelo es %d\n", nom[j+2], getpid(), getppid(), pids[1], pids[0]);
			
						if(j==2)//estamos en Z
						{
							alarm(tiempo);
							pause();//esperamos hasta que llegue la alarma
							switch(letra)
							{
					  		case 'A':
					  			kill(pids[1],SIGUSR2);
					    	break;
					  		case 'B':
					  			kill(pids[2],SIGUSR2);
					    	break;
					  		case 'X':
					  			kill(pids[3],SIGUSR1);
					    	break;
					  		case 'Y':
					  			kill(pids[4],SIGUSR1);
					    	break;
							}

						}
			 			pause();//recibo la señal kill del switch y mato a los hijos
			  			printf("Soy %c (%d) y muero.\n", nom[j+2], getpid());
			  			exit(0);
		    		}
		      
				}
		    	/* El proceso padre espera a que mueran todos los hijos */
		    	for (j = 0; j < 3; j++)
		    	{
		     		wait();
		    	}
		    	/* Saco el mensaje de que el padre muere */
		    	printf("Soy B (%d) y muero.\n", pids[0]);
		    	exit(0);
			}
		}
	}
	else
	{
	  printf ("Error en los parámetros\n");
	}
}

void lsSenal()
{
	execlp("ls","ls","-la",NULL);//llamar a ls
}

void pstreeSenal()
{
	execlp("pstree","pstree","-p",NULL);//llamar a pstree

}

void alarmSenal()
{
}
