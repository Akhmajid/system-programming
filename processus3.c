TUBES ANONYMES
===========================

Recopiez le code source de votre programme (exo1.c) ici.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>


int main (int argc, char *argv[]){
	printf("Je suis le pere %d\n", getpid());
	pid_t pid;
	
	
	int tube[2]; 
	srand(time(NULL));
	if(pipe(tube) !=0){
		printf("Erreur creation pipe\n");
	}
	if ((pid=fork())<0){
		printf("Erreur fork\n");
		exit(EXIT_FAILURE);
		}
	else {
		if (pid==0){
				close(tube[0]);
				int a;
				a=rand()%100;
				printf("Le fils %d ecrit %d dans le tube. \n", getpid(), a);
				write(tube[1],&a, sizeof(int));
				close(tube[1]);
				exit(EXIT_SUCCESS);
			
			}
			
		else {
			
			if ((pid=fork())==0){
				int buf;
				close(tube[1]);
				read(tube[0],&buf, sizeof(int));
				printf("Le fils %d a lu %d dans le tube et se termine. \n", getpid(), buf);
				close(tube[0]);
				exit(EXIT_SUCCESS);
				
				}
			}
		
		}
		
		
		return 0;
		}
			
	
	TUBES NOMMES
=========================

Recopiez le code source de vos programmes (exo2a.c et exo2b.c) ici.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


int main (int argc, char *argv[]){
	
	int entreeTube;
	srand(time(NULL));
	int a=rand()%100;
	
    if(mkfifo(argv[1], S_IRUSR|S_IWUSR) != 0) {
        fprintf(stderr, "Impossible de créer le tube nommé.\n");
        exit(EXIT_FAILURE);
    }


    if((entreeTube = open(argv[1], O_WRONLY)) == 0){
    	fprintf(stderr, "Impossible d'ouvrir l'entrée du tube nommé.\n");
        exit(EXIT_FAILURE);
    }
    
    write(entreeTube, &a, sizeof(int));



    return EXIT_SUCCESS;
    }


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


int main (int argc, char *argv[]){
		int sortieTube;


    int a;


    if((sortieTube = open (argv[1], O_RDONLY)) == -1) {
        fprintf(stderr, "Impossible d'ouvrir la sortie du tube nommé.\n");
        exit(EXIT_FAILURE);
    }


    read(sortieTube, &a, sizeof(int));

        printf("%d \n", a);


    return EXIT_SUCCESS;

}





 OUVERTURES MULTIPLES ET PARTAGE D'OFFSET
=====================================================

Recopiez le code source de votre programme (exo3.c) ici.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


int main (int argc, char *argv[]){
	if ( argc !=2){
		printf("Nombre d'argument incorrect\n");
		exit (EXIT_FAILURE);
	}
	
	pid_t pid;
	printf("Le pere %d commence\n", getpid());
	
	int fic;
	char lu;
	int i;
	int j;
	
	if ((fic=open(argv[1],O_RDONLY))==-1){
		printf("Impossible d'ouvrir %s\n",argv[1]);
		exit(EXIT_FAILURE);
	}
	
	for (i=0; i<2;i++){
		if ((pid =fork())<0){
			printf("Erreur fork\n");
			exit(EXIT_FAILURE);
		}
		else{
			if (pid==0){
				if (i==0){
					printf("Le premier fils %d commence\n",getpid());
				}
				else{
					printf("Le deuxieme fils %d commence\n",getpid());
				}
				for (j=0;j<3;j++){
					
					if(read(fic,&lu,sizeof(char))!=0){ 
						printf("Le fils %d  de  pere %d a lu %c\n",getpid(),getppid(),lu);
						//exit(EXIT_SUCCESS);
						
						}
					else{
						printf("Erreur lecture\n"); 
						close(fic);
						exit(EXIT_FAILURE);
				}
					
					
				}
				printf("Le fils %d se termine\n",getpid());
				exit(EXIT_SUCCESS);
				
			}
			
		}
		wait(NULL);
		if(i==0){
			if(read(fic,&lu,sizeof(char))!=0){ 
						printf("%d a lu %c\n",getpid(),lu);
			}
		}
		
	}
	close(fic);
	if ((pid =fork())<0){
			printf("Erreur fork\n");
			exit(EXIT_FAILURE);
		}
		else{
			if (pid==0){
				printf("Le troisieme fils %d commence\n",getpid());
				fic=open(argv[1],O_RDONLY);
				for (j=0;j<4;j++){
					
					if(read(fic,&lu,sizeof(char))!=0){ 
						printf("Le fils %d  de  pere %d a lu %c\n",getpid(),getppid(),lu);
						//exit(EXIT_SUCCESS);
						
						}
					else{
						printf("Erreur lecture\n"); 
						close(fic);
						exit(EXIT_FAILURE);
				}
					
					
				}
				printf("Le fils %d se termine\n",getpid());
				exit(EXIT_SUCCESS);
			}
			}
	wait(NULL);
	printf("Le pere %d se termine\n",getpid());
	close(fic);
	exit(EXIT_SUCCESS);
}

 INCOHERENCES DUES AUX COMMUTATIONS
===============================================



Recopiez le code source de votre programme (exo4b.c) ici.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


int main (int argc, char *argv[]){
	if(argc!=2){
		printf("Paramère attendu : nom du fichier\n");
		exit(EXIT_FAILURE);
	}
	fork();
	execl("exo4a","exo4a",argv[1],NULL);
	
}



Recopiez un résultat inattendu ici et jsutifiez le précisèment

15223 : premiere ecriture

Le résulat est du a une commutation: en effet un processus exécute le code exo4a, test si le fichier existe. le processus voit que le fichier n'existe pas mais il y a une commutation avant qu'il nait le temps de crée le fichier. Le deuxième processus exécute alors à son tour le code exo4a. Il test l'existance du fichier et comme il n'a pas encore été créer par le prremier processus, le fichier n'existe pas encore. Il créer alors le fichier et affiche son pid et "premiere écriture". Il termine ensuite son exécution. Le premier processus reprend donc et créer le fichier en écrasant l'ancien pour y écrire son pid et "premiere écriture" puis se termine. C'est pour cela qu'on a l'impression qu'un seul processus a été exécuté.


Recopiez le code source modifié de votre programme (exo4a.c et/ou exo4b.c) ici. 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


int main (int argc, char *argv[]){
	if(argc!=2){
		printf("Paramère attendu : nom du fichier\n");
		exit(EXIT_FAILURE);
	}
	if(fork()==0){
		
		execl("exo4a","exo4a",argv[1],NULL);
	}
	wait(NULL);
	execl("exo4a","exo4a",argv
