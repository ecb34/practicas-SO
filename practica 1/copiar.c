#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>


int main(int argc, char* argv[]){

  if(argc==3)
  {
    
    char archivo1[20];
    strcpy(archivo1,argv[1]); // se copia en archivo1 el nombre del fichero origen
    char archivo2[20];
    strcpy(archivo2,argv[2]); // se copia en el archivo2 el nombre del fichero destino.
    
    int descriptores[] = {0,0}; // se inicializa el vector descriptores que se usa para la tuberia
    int df; //comprobador de que se haya abierto bien el archivo

    pipe(descriptores); //inicializacion tuberia para fichero origen y destino.
    
    int pid = fork();
    
    if(pid>0)
    { // es el padre descriptores[1] - leer archivo escribir en pipe
      df = open(archivo1,O_RDONLY);
      if(df<0)
      {
		printf("No encuentro *%s*.\n",archivo1);
      }
      else
      { // archivo abierto para leer
		char bufferlec[10]; 
		int tamanyo = read(df,bufferlec,9); //leo 9 bytes del archivo origen
	
		if(tamanyo == -1)
		{
	  		perror("Error de lectura.\n");
		}
		else if(tamanyo==0)
		{
	  		perror("Archivo vacio.\n");
		}
		else
		{ // se escribe en la tuberia
			write(descriptores[1],bufferlec,strlen(bufferlec)); // escribo en tuberia
			int i=0;	
			while(i<10) // borro contenido de buffer
	  		{
	  			bufferlec[i]='\0';
	  			i++;
	  		}
	  		do{ // mientras no de error la lectura del fichero y el buffer no este vacío, se sigue escribiendo en la tuberia.
	  			write(descriptores[1],bufferlec,strlen(bufferlec));
	  			i=0;
	  			while(i<10)
	  			{
	  				bufferlec[i]='\0';
	  				i++;
	  			}
	  			
	  		}while(read(df,bufferlec,9)>=0 && strlen(bufferlec)>0);
		}
	
		// fin lectura
		close(df);
    }
           
}
   else if(pid == 0)
   { // es el hijo descriptores[0] - leer en pipe escribir en archivo
      df = creat(archivo2,0777); 
      if(df<0) // error abriendo archivo
      { 
		perror("Error al crear\n");
		exit(0);
      }
      else
      { // archivo abierto para escribir
		char bufferlec[10];
		int i;
		while(read(descriptores[0],bufferlec,9)>=0 && strlen(bufferlec)>0) // mientras se pueda leer del buffer, se sigue leyendo.
		{
	  		write(df,bufferlec,strlen(bufferlec)); // se escribe en el fichero destino.
	 		i=0;
	  		while(i<10)
	  		{
	    		bufferlec[i]='\0';
	    		i++;
	  		}
		}
	
		close(df);
      }
    }
    else
    {
      printf("Error en el fork()\n");
    }
  }
  else
  {
    perror("Cantidad de argumentos erroneos.\n");
  }
 
  exit(0);
}
