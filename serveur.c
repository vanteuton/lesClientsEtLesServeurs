#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "requete.h"

// definition des message de structures request et response
// partage de l'identifiant externe de la file, CLE = 160

//_______________________________________________________________________________________________________________
// FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS
//_______________________________________________________________________________________________________________
  

int estPremier (int nombre)
{
  int i, reponse = 1;
  for(i = nombre - 1; i != 1; i-- ) {
	  if( nombre % i == 0)
	  {
		reponse = 0;
		break;
	  }
  }
  return reponse;
}

// Calcul de la surface du disque
float aireDisque(float r)
{
  float aire= (2 * PI * r * r);
  return aire;
}
// Calcul perimètre cercle
float perimetreCercle(float r)
{
  float perimetre = (PI * 2 * r);
  return perimetre;
}


//_______________________________________________________________________________________________________________
// MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN MAIN
//_______________________________________________________________________________________________________________


int main() {
	int msqid;
	Request requete;
	Response resultat;
	
	//ouverture la file de msg CLE, la cree si elle nexiste pas
	if ((msqid = msgget((key_t) CLE, IPC_CREAT|0750)) == -1) {
		// affichage de l'erreur provenant du dernier appel system
		perror("msgget");
		exit(1);
	}
	
	while (1) {    
		// reception des msg client
		// de taille structure Request - 4 (taille de la variable type, un int est code sur 4 octets ) 
		// type de msg = 1 ( msg de type req. de calcul)
		if (msgrcv(msqid, &requete, sizeof(Request)-sizeof(long), 1, 0) == -1) {
			perror("msgrcv");
			exit(2);
		}
		
		resultat.type = (int) requete.PID;
			
		switch (requete.choix) 
	   {
		  case 1:
			printf("op=%d, pid client=%d\n",requete.choix, requete.PID);
				if(estPremier(requete.nbPremier) == 1)
					resultat.resNbPremier = 1;
				else
					resultat.resNbPremier = 0;
		  case 2:
			printf("op=%d, pid client=%d\n",requete.choix, requete.PID);
				resultat.resCercle = perimetreCercle(requete.circCercle);
				break;
		  case 3:
			printf("op=%d, pid client=%d\n",requete.choix, requete.PID);
				resultat.resSurfCercle = aireDisque(requete.surfCercle);
				break;
		  default:
				break;
		}
    
  	//resultat.mtype = 1;
    
    
		//envoi du msg resultat au client identifié par son pid 
		if (msgsnd(msqid, &resultat, sizeof(Response)-sizeof(long), 0) == -1) {
			perror("msgsnd");
			exit(3);
		}
	}
	
	msgctl((key_t) CLE,0,NULL);
	return 0;
}
