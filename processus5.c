EXERCICE 1 : Première et deuxième écritures
===========================================

Recopiez le code source de votre programme (exo1.c) ici.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[]){
	if(argc!=2){
		printf("Paramère attendu : nom du fichier\n");
		exit(EXIT_FAILURE);
	}
	struct stat sb;
	int fic=open("tata",O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR);
		lseek(fic,0,SEEK_SET);
		lockf(fic,F_LOCK,0);
	
	if(stat(argv[1],&sb)==-1){ // Le fichier n'existe pas, on écrit dedans
		int fichier=open(argv[1],O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR);
		
		
		dup2(fichier,1);
		close(fichier);
		printf("%d : premiere ecriture\n",getpid());
		lseek(fic,0,SEEK_SET);
		lockf(fic,F_ULOCK,0);
		close(fic);
		exit(EXIT_SUCCESS);
		
	}else{ // Le fichier existe, on écrit à la suite

		int fichier=open(argv[1],O_APPEND|O_WRONLY);
		
		dup2(fichier,1);
		close(fichier);
		printf("%d : deuxieme ecriture\n",getpid());
		lseek(fic,0,SEEK_SET);
		lockf(fic,F_ULOCK,0);
		close(fic);
		
		exit(EXIT_SUCCESS);
	}
	exit(EXIT_SUCCESS);
}

Atchoum et Dormeur
===============================

:

Recopiez le code source de vos programmes (exo2a.c et exo2b.c) ici.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){

	srand(getpid());
	int i,k;
	
	int verrou=open("verrou",O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
	lseek(verrou,0,SEEK_SET);
	lockf(verrou,F_LOCK,0);
	
	printf("Dormeur rentre aux toilettes\n");
	sleep(1);
	for(k=rand()%10;k>=0;k--){
		printf("Dormeur sort des toilettes\n");
		lseek(verrou,0,SEEK_SET);
		lockf(verrou,F_ULOCK,0);

		for(i=rand()%6;i>=0;i--){
			printf("zzzz\n");
			sleep(1);
		}
		//int verrou=open("verrou",O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
		lseek(verrou,0,SEEK_SET);
		lockf(verrou,F_LOCK,0);	
		printf("Dormeur rentre aux toilettes\n");
		sleep(1);
	}
	printf("Dormeur sort des toilettes\n");

	lseek(verrou,0,SEEK_SET);
	lockf(verrou,F_ULOCK,0);
	close(verrou);

	exit(EXIT_SUCCESS);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){

	srand(getpid());
	int i,k;
	
	int verrou=open("verrou",O_CREAT|O_RDWR,S_IRUSR|S_IWUSR);
	

	for(k=rand()%10;k>=0;k--){
		for(i=rand()%4;i>=0;i--){
			printf("Atchoum !\n");
			sleep(1);
		}
		lseek(verrou,0,SEEK_SET);
		lockf(verrou,F_LOCK,0);
		printf("Atchoum rentre aux toilettes\n");
		sleep(1);
		printf("Atchoum sort des toilettes\n");
		lseek(verrou,0,SEEK_SET);
		lockf(verrou,F_ULOCK,0);
		for(i=rand()%4;i>=0;i--){
			printf("Atchoum !\n");
			sleep(1);
		}
	}

	
	close(verrou);

	exit(EXIT_SUCCESS);
}

Allons faire un tour à la piscine...
=================================================

 

Recopiez le code source de votre programme (exo3a.c) ici.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>


void prise(char* fichier, int id){
	
	int fic;
	int n;
	fic = open(fichier, O_RDWR,S_IRUSR|S_IWUSR);
		lseek(fic,0,SEEK_SET);
		lockf(fic,F_LOCK,0);
		
	read(fic,&n, sizeof(int));
	printf("Il y a %c %s\n", n,fichier);
	
	while (n==0){
		
		printf("Il n'y a plus de cabine disponible, attendre une seconde\n");
		printf("Baigneur %d attend une seconde\n",id);
		lseek(fic,0,SEEK_SET);
		lockf(fic,F_ULOCK,0);
		
		sleep(1);
		lseek(fic,0,SEEK_SET);
		lockf(fic,F_LOCK,0);
		read(fic,&n, sizeof(int));
		}
	n=n-1;
	
	lseek(fic,0,SEEK_SET);
	write(fic, &n, sizeof(int));
	printf("Baigneur %d prend %s \n Il reste %c %s disponibles\n",id,fichier,n,fichier);
	lseek(fic,0,SEEK_SET);
	lockf(fic,F_ULOCK,0);		
	close(fic);
	
	return;
}

void liberation (char * fichier,int id){
	
	int fich;
	int n;
	fich = open(fichier, O_RDWR,S_IRUSR|S_IWUSR);
		lseek(fich,0,SEEK_SET);
		lockf(fich,F_LOCK,0);
		
	read(fich,&n, sizeof(int));
	//printf("Il y a %c %s\n", n,fichier);

		
	n=n+1;
	
	lseek(fich,0,SEEK_SET);
		
	write(fich, &n, sizeof(int));
	printf("Baigneur %d libère %s \n Il reste %c %s disponibles\n",id,fichier,n,fichier);
	lseek(fich,0,SEEK_SET);
	lockf(fich,F_ULOCK,0);		
	close(fich);
	
	return;
}

int main(int argc, char *argv[]){
	if(argc!=2){
		printf("Paramère attendu : identifiant du baigneur\n");
		exit(EXIT_FAILURE);
	}
	int id = atoi(argv[1]);
	
	
	printf("Baigneur %d arrive a la piscine\n",id);

	prise("cabines",id);
	prise("paniers",id);

	printf("Baigneur %d se change \n",id);

	liberation("cabines",id);

	printf("Baigneur %d se baigne \n",id);
	
	prise("cabines",id);
	
	printf("Baigneur %d se change \n",id);

	liberation("paniers",id);
	liberation("cabines",id);
	
	printf("Baigneur %d part de la piscine \n",id);
	return 0;
	}



Recopiez le code source de votre programme (exo3b.c) ici.
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>



int main(int argc, char *argv[]){
	if(argc!=4){
		printf("Erreur de paramètre\n");
		exit(EXIT_FAILURE);
	}
	int i;
	char id[100];
	pid_t pid;
	int nbB = atoi(argv[1]);
	int nbC= atoi(argv[2]);
	int nbP= atoi(argv[3]);

	int fic1 = open("cabines",O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR);
    	write(fic1, &nbC, sizeof(int));
   	close(fic1);
    
   	int fic2 = open("paniers",O_CREAT|O_TRUNC|O_WRONLY,S_IRUSR|S_IWUSR);
   	write(fic2, &nbP, sizeof(int));
    	close(fic2);

	for (i=0;i<nbB;i++){
		
		pid=fork();
		if (pid==0){

			sprintf (id‚ "%d"‚ i);
			execl("exo3","exo3",id,NULL);
		}
	}
	
	for (i=0;i<nbB;i++){
		wait(NULL);
		
	}
	return 0;
	}


