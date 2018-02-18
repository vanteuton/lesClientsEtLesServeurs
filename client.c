#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "requete.h"

// definition des message de structures request et response
// partage de l'identifiant externe de la file, CLE = 142 
  
int main(void){
	int msqid;
	Request requete;
	Response resultat;
	
	//ouverture de la file CLE
	(msqid = msgget( (key_t) CLE, 0)) < 0 ? perror("msgget") : "";
	
	//menu
	
	int choixMenu = 0;	
	printf("\n--------------------------------------------- \n");
	printf("---------------      MENU      --------------\n");
	printf("--------------------------------------------- \n \n");
	printf("Que voulez vous effectuer \n");
	printf("---- 1 Nombre premier \n");
	printf("---- 2 Circonference Cercle \n");
	printf("---- 3 Surface Cercle \n");
	printf("Votre choix?\n\n");
	scanf("%d", &choixMenu);
	
	printf("\n");
	
  //On stock la valeur à calculer dans un champ différent pour permettre au serveur de déterminer le calcul à faire
	switch (choixMenu) 
	{
		case 1:
			printf("Vous avez choisis le calcul des nombres premiers. \n");
			requete.choix = 1;
			//saisi au clavier
			printf("Veuillez entrer un nombre. \n");
			scanf("%d", &requete.nbPremier);
						
			break;
		case 2:
			printf("Vous avez choisis le calcul de la circonference d'un cercle.\n");
			//saisi au clavier
			printf("Veuillez entrer un nombre\n");
			requete.choix = 2;
			scanf("%f", &requete.circCercle);
			
			break;
		case 3:
			printf("Vous avez choisis le calcul de la surface d'un cercle.\n");
			//saisi au clavier
			printf("Veuillez entrer un nombre\n");
			requete.choix = 3;
			scanf("%f", &requete.surfCercle);
			
			break;
		default:
			printf("Vous n'avez pas demandé une opération disponible. \n1 - Calcul des nombres premiers. \n2 - Calcul de la circonférence d'un cercle.\n3 - Calcul de la surface d'un cercle");
			break;
	}


		// utilisation du pid  du processus client 
		// pour que le serveur puisse l'identifier de facon unique 
		requete.PID = getpid(); // pour la reponse
		requete.type = 1;
		
		
		//envoi du msg au serveur
		// de taille structure Request - 4 (taille de la variable type, un long est code sur 4 octets ) 
		msgsnd(msqid, &requete, sizeof(Request)-sizeof(long), 0) == -1 ? perror("msgsnd") : "";
		
		msgrcv(msqid, &resultat, sizeof(Response)-sizeof(long), getpid(), 0) == -1 ? perror("msgrcv") : "";
		
		//affichage du resultat
    printf("Réponse du serveur\n\n");
  	//affichage du resultat
		switch(requete.choix){
			case 1:
				printf("Le resultat est %s  \n \n", (char)resultat.resNbPremier == 1 ? "premier" :"non premier");
				break;
			case 2:
				printf("Le resultat est %.2f  \n", resultat.resCercle);
				break;
			case 3:
				printf("Le resultat est %.2f  \n", resultat.resSurfCercle);
				break;
		}
		return 0;
	}
