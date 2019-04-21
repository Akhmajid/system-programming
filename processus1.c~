#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h> 

int main(){


	pid_t pid;
	
	int tube1[2];
	int tube2[2];
	
	int i;
	if (pipe(tube1)!=0){
	printf("Erreur creation pipe\n");
	}
	
		
	if (pipe(tube2)!=0){
	printf("Erreur creation pipe\n");
	}
	
	
	
	 
	pid=fork();
	
	if(pid ==0){
		printf("je suis le premier fils et j'écris dans le tube1 et je lis dans le tube 2\n");
		int a = 1;
		int b;
		close(tube1[0]);
		close(tube2[1]);
		
		write(tube1[1], &a, sizeof(int));
		
		printf("Coco\n");
		srand(getpid());
		int attente = rand()%(3-1) +1;
		sleep(attente);
		read(tube2[0], &b, sizeof(int));
		while (b==2){

			
			printf("Coco \n");
			write(tube1[1], &a, sizeof(int));
			attente = rand()%(3-1) +1;
			sleep(attente);
			read(tube2[0], &b, sizeof(int));
		}
		close(tube1[1]);
		close(tube2[0]);
		exit(EXIT_SUCCESS);
	}
		
	pid=fork();	
	if (pid==0){
		
		printf("je suis le second fils je lis dans le tube1 et j'écris dans le tube 2\n");
		int b = 2;
		int a;
		close(tube1[1]);
		close(tube2[0]);
		
		read(tube1[0], &a, sizeof(int));
		while(a==1){
			printf("jaco\n");
		
			write(tube2[1], &b, sizeof(int));
			srand(getpid());
			int attente = rand()%(3-1) +1;
			sleep(attente);
			read(tube1[0], &a, sizeof(int));
		}
		close(tube1[1]);
		close(tube2[0]);
		exit(EXIT_SUCCESS);
		
	}	 
	
	srand(time(NULL));
	int v=rand()%(30-20)+20;
	printf("Durée du père: %d\n", v);
	sleep(v);
	i=10;
	close(tube1[0]);
	close(tube2[0]);
	
	write(tube2[1], &i, sizeof(int));
	write(tube2[1], &i, sizeof(int));
	close(tube1[1]);
	close(tube2[1]);
	wait(NULL);
	wait(NULL);
	
	return 0;	
}

 Barrière de synchronisation
========================================

Recopiez le code source de votre programme (exo2.c) ici.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h> 


int main(int argc,char **argv){
	if (argc != 2){
		printf("Parametre attendu : nombre de fils\n");
		exit(EXIT_FAILURE);
	}
	
	pid_t pid;
	int a=0;
	int tube1[2];
	int tube2[2];
	
	int attente, i;
	if (pipe(tube1)!=0){
	printf("Erreur creation pipe\n");
	}
	
		
	if (pipe(tube2)!=0){
	printf("Erreur creation pipe\n");
	}



	for (i=1;i<=atoi(argv[1]);i++){
		pid =fork();
		if (pid==0){

			srand(getpid());
			attente=rand()%(15-5)+5;
			printf("Le fils %d exécute son bloc A en %d seconde et attend les autres\n",i, attente);
			sleep(attente);
			
			write(tube1[1],&i, sizeof(int));
			//on attend l'autorisation du père quand tous les fils ont fini:
			read(tube2[0],&a, sizeof(int));
			write(tube2[1], &a, sizeof(int));
			printf("Le fils %d passe la barrière\n",i);
			close(tube1[1]);
			close(tube2[0]);
			close(tube1[0]);
			close (tube1[0]);
			exit(EXIT_SUCCESS);
		}

	}


	
	while (a!=atoi(argv[1])){
		
		read(tube1[0],&a, sizeof(int));
		write(tube1[1],&a, sizeof(int));

	}
	printf ("Le père débloque ses fils et lève la barrière\n");
	write(tube2[1], &a, sizeof(int));

	close(tube1[0]);
	close(tube2[1]);
	close(tube1[1]);
	close(tube2[0]);

	for (i=0;i<atoi(argv[1]);i++){
		wait(NULL);
	}

	return 0;
}

 Verrous vs. synchronisations
=========================================

Question 1 : 

Recopiez le code source de votre programme (exo3a.c) ici.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void fils(char *chaine){
	
	int fic1=open("fichier",O_CREAT|O_APPEND|O_WRONLY,S_IRUSR|S_IWUSR);
	lseek(fic1, SEEK_SET, 0);
	lockf(fic1, F_LOCK, 0);
	dup2(fic1,1);
	
	printf("%d a ecrit %s\n",getpid(),chaine);
	fflush(stdout);

	srand(getpid());
	int attente=rand()%4;
	fprintf(stderr,"%d attente de %d\n",getpid(),attente);
	sleep(attente);

	int fic2=open("copie",O_CREAT|O_APPEND|O_WRONLY,S_IRUSR|S_IWUSR);
	lseek(fic2, SEEK_SET, 0);
	lockf(fic2, F_LOCK, 0);	
	dup2(fic2,1);
	
	printf("%d a ecrit %s\n",getpid(),chaine);
	lseek(fic1, SEEK_SET, 0);
	lockf(fic1, F_ULOCK, 0);
	lseek(fic2, SEEK_SET, 0);
	lockf(fic2, F_ULOCK, 0);
	close(fic1);
	close(fic2);
	exit(EXIT_SUCCESS);
}

int main(int argc,char **argv){

	int i;
	pid_t pid;
	system("rm -f fichier copie");

	for(i=1;i<argc;i++){
		if((pid=fork())<0){
			printf("Erreur fork\n");
			exit(EXIT_FAILURE);
		}else if(pid==0){
			fils(argv[i]);
		}
	}
	exit(EXIT_SUCCESS);
}
Question 2 :

Recopiez le code source de votre programme (exo3b.c) ici.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void fils(char *chaine){
	
	int fic=open("fichier",O_CREAT|O_APPEND|O_WRONLY,S_IRUSR|S_IWUSR);
	
	dup2(fic,1);
	close(fic);
	printf("%d a ecrit %s\n",getpid(),chaine);
	fflush(stdout);

	srand(getpid());
	int attente=rand()%4;
	fprintf(stderr,"%d attente de %d\n",getpid(),attente);
	sleep(attente);

	fic=open("copie",O_CREAT|O_APPEND|O_WRONLY,S_IRUSR|S_IWUSR);
	
	dup2(fic,1);
	close(fic);
	printf("%d a ecrit %s\n",getpid(),chaine);
	
	
	
	
	exit(EXIT_SUCCESS);
}

int main(int argc,char **argv){
	int tube1[2];
	int tube2[2];
	pipe(tube1);
	pipe(tube2);
	int i,a=1;
	pid_t pid;
	system("rm -f fichier copie");

	for(i=1;i<argc;i++){
		if((pid=fork())==0){
			close(tube1[1]);
			close(tube2[0]);
			
			read(tube1[0],&a,sizeof(int));
			write(tube2[1],&a,sizeof(int));
			
			close(tube1[0]);
			close(tube2[1]);
			
			fils(argv[i]);
			
			
		}
		else{
			close(tube1[0]);
			close(tube2[1]);
			
			write(tube1[1],&a,sizeof(int));
			read(tube2[0],&a,sizeof(int));
			
			close(tube1[1]);
			close(tube2[0]);
			}
	
	}
	exit(EXIT_SUCCESS);
}
