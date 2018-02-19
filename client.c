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
  
 // char * to int c
int string2int(char stringNumber[]) {
  // 1.
  if (!stringNumber) {
    printf("Parameter error\n");
    return 0;
  }
  int number = 0;
  int i = 0;
 
  // 3.
  int stringLength = strlen(stringNumber);
 
  while (i < stringLength) {
    int digit = stringNumber[i] - '0';
    if (digit < 0 || digit > 9) {
      printf("Invalid character '%c' on the position '%d'\n", stringNumber[i],(i));
      return 0;
    }
    number *= 10;
    number += digit;
    i++;
  }
 
  return number;
  
}
 
float stringToFloat(char *string)
{
    float result= 0.0;
    int len = strlen(string);
    int dotPosition = 0;

    for (int i = 0; i < len; i++)
    {
        if (string[i] == '.')
        {
          dotPosition = len - i  - 1;
        }
        else
        {
          result = result * 10.0 + (string[i]-'0');
        }
      }

      while (dotPosition--)
      {
        result /= 10.0;
      }

    return result;
}
 
int main(int argc, char *argv[]){
	int msqid;
	Request requete;
	Response resultat;
	
	//ouverture de la file CLE
	(msqid = msgget( (key_t) CLE, 0)) < 0 ? perror("msgget") : "";
  //On stock la valeur à calculer dans un champ différent pour permettre au serveur de déterminer le calcul à faire
	switch (string2int(argv[1])) 
	{
		case 1:
			printf("\n\nVous avez choisis le calcul des nombres premiers. \n\n");
			requete.choix = 1;
			requete.nbPremier = string2int(argv[2]);  
			printf("Nombre entré : %d\n\n", requete.nbPremier);
			break;
		case 2:
			printf("\n\nVous avez choisis le calcul de la circonference d'un cercle.\n\n");
			requete.choix = 2;
			requete.circCercle = stringToFloat(argv[2]);
			printf("Nombre entré : %.2f\n\n", requete.circCercle);
			
			break;
		case 3:
			printf("\n\nVous avez choisis le calcul de la surface d'un cercle.\n\n");
			requete.choix = 3;
			requete.surfCercle = requete.circCercle = stringToFloat(argv[2]);
			printf("Nombre entré : %.2f\n\n", requete.surfCercle);
			
			break;
		default:
			printf("Vous n'avez pas demandé une opération disponible. \n1 - Calcul des nombres premiers. \n2 - Calcul de la circonférence d'un cercle.\n3 - Calcul de la surface d'un cercle");
			break;
	}


		// utilisation du pid  du processus client 
		// pour que le serveur puisse l'identifier de facon unique 
		requete.PID = getpid(); // pour la reponse
		printf("PID -> %d \n",requete.PID);
		printf("msqid -> %d \n",msqid);
		requete.type = 1;
		
		
		//envoi du msg au serveur
		// de taille structure Request - 4 (taille de la variable type, un long est code sur 4 octets ) 
		msgsnd(msqid, &requete, sizeof(Request)-sizeof(long), 0) == -1 ? perror("msgsnd") : "";
		printf("msgsnd envoye\n");
		msgrcv(msqid, &resultat, sizeof(Response)-sizeof(long), getpid(), 0) == -1 ? perror("msgrcv") : "";
		printf("msgrcv reçu\n");
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
