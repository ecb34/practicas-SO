#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

int main(int argc, char *argv[]){
	int x, y, pid, i, k;
	int shmidx, shmidy;
	int *idx, *idy;

	if(argc == 3)
	{
		x = atoi(argv[1]); // los hijos en vertical
		y = atoi(argv[2]); // hijos en horizontal

		// reservo memoria de tamaño sizeof(int)*y, es un vector de y enteros, y almaceno el id de la memoria compartida en shmidy
		shmidy = shmget(IPC_PRIVATE, sizeof(int)*y, IPC_CREAT|0666);

		// guardo la direccion de comienzo de la memoria reservada en idy
		idy = (int *) shmat(shmidy, 0, 0);

		// reservo igualmente memoria para el vector de enteros x, y almaceno el id de la memoria compartida en shmidx
		shmidx = shmget(IPC_PRIVATE, sizeof(int)*(x+1), IPC_CREAT|0666);
		idx = (int *) shmat(shmidx, 0, 0);

		idx[0] = getpid();  // guardo en la primera pos el pid del padre
		for(i = 1; i <= x; i++)
		{ // creo x hijos
			pid = fork();
			if(pid != 0)
			{
				idx[i] = pid; // voy guardando los pids de cada hijo en vertical
				wait(NULL);
				break; 		// el padre se sale
			}
		}
		if(i == x + 1)
		{
			// el ultimo hijo.
			for(i = 1; i <= y; i++)
			{// creo los y hijos en horizontal
				pid = fork();
				if(pid == 0)
				{// los hijos en horizontal muestran su pid y el de sus padres
					idy[i-1] = getpid();
					printf("Soy el subhijo %d. Mis padres son ", getpid());
					for(k = 0; k <= x; k++)
					{//imprimo todos los padres
						printf("%d", idx[k]);
						if(k != x)
						{
							printf(", ");
						}				
					}
					printf("\n");
					sleep(15);
					break;
				}
			}
			if(i == y + 1)
			{
				wait(NULL);
			}
		}
		else
		{
			if(i == 1)
			{// el superpadre muestra todos sus hijos
				printf("Soy el superpadre (%d) : mis hijos finales son: ", getpid());
				
				for(i = 1; i <= x; i++)
				{
					printf("%d,", idx[i]);
				}
				for(i = 0; i < y; i++)
				{
					printf("%d", idy[i]);
					if(i != y-1)
					{
						printf(",");
					}
				}
				printf("\n");
			}
		}
	}
	return 0;
}
