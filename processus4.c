EXERCICE 1 : FORK
=================
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void creerChaine(int i, int n){
	pid_t pid;
	if (i==1){
	pid=fork();
		if (pid==0){
			printf("pid du pere %d : %d\n",n+1,getppid());
			printf("pid du fils %d : %d\n",n+1,getpid());
			exit(1);
		}
	}	
	if (i<=n){
		pid = fork();
		if (pid == 0){
			printf("pid du pere %d : %d\n",i,getppid());
			printf("pid du fils %d : %d\n",i,getpid());
			creerChaine(i+1,n);
			}
		
	}
	
}

int main (int args, char *argv[]){
	int n=atoi(argv[1]);
	printf("je suis le pere : 0  pid: %d \n", getpid());
	creerChaine(1, n);
	return 0;
	}



WAIT ET VALEUR DE RETOUR
=====================================

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


void waitVal(){
	int a,b,e;
	pid_t pid;
	pid=fork();
	
		if (pid==0){
			printf("Veuillez saisir deux entiers:\n");
			scanf("%d%d", &a,&b);
			
			if (a+b>10){
				exit(EXIT_SUCCESS);}
			else{
				exit(EXIT_FAILURE);
		}
		}
		wait(&e);
		if (WIFEXITED(e)){
			if (WEXITSTATUS(e)==EXIT_SUCCESS){
				printf("La somme des entiers est supérieur à 10\n");}
			else{
				printf("La somme des entiers n'est pas supérieur à 10\n");}
		}
				
}

int main (){
	waitVal();
	return 0;
	}

 MULTI-GREP
=======================

Recopiez le code source de votre programme (exo3.c) ici.
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>



int main (int args, char *argv[]){
	pid_t pid;
	int i=2;
	while(i<args){
		pid=fork();
		if(pid == 0){
			printf(" fichier numéro:%d\n",i-1);
			//printf("%s\n",argv[i]);
			execl("/bin/grep", "grep", argv[1], argv[i], NULL);
			}
		i++;
	}


	return 0;
	}


 MULTI-GREP CONTRAINT
=================================

Recopiez le code source de votre programme (exo4.c) ici.
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXFILS 1

int main (int args, char *argv[]){
	pid_t pid;
	int i=2;
	printf("%s\n",argv[1]);
	while((i<args) && (i-2<MAXFILS)){
		pid=fork();
		if(pid == 0){
			printf(" fichier numéro:%d\n",i-1);
			printf("%s\n",argv[i]);
			execl("/bin/grep", "grep", argv[1], argv[i], NULL);
			}
		i++;
	}
	
	while (i<MAXFILS){
		printf(" fichier numéro:%d\n",i-1);
		printf("%s\n",argv[i]);
		execl("/bin/grep", "grep", argv[1], argv[i], NULL);
		}
		
	
	return 0;
	}

