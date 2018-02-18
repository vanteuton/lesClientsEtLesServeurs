#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>









// definition des message de structures request et response
// partage de l'identifiant externe de la file, CLE = 160

#define CLE 160

// structure message pour la requete
struct Request
{
    long mtype;
    int operationDemandee;
    float valeurEnvoyee;   //pour le nombre premier
    pid_t PID;
};
typedef struct Request Request;

// structure message pour la reponse
struct Response
{
    long mtype;
    char result[40];
};


typedef struct Response Response;

  
// char * to int c
int string2int(char stringNumber[]) {
  // 1.
  if (!stringNumber) {
    printf("Parameter error\n");
    return 0;
  }
  char numberSign = stringNumber[0];
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


int main( int argc, char *argv[] )  {

  	int opeDemandee = string2int(argv[1]);  
    float valeurDemandee = stringToFloat(argv[2]);

    int msqid;
    Request request;
    Response resultat;

  	printf("CLIENT CLIENT CLIENT CLIENT CLIENT CLIENT CLIENT CLIENT CLIENT CLIENT \n");
    //ouverture de la file CLE
    if ((msqid=msgget((key_t) CLE, 0)) < 0) {
        // affichage de l'erreur provenant du dernier appel system
        perror("msgget");
        exit(1);
    }
    printf("msqid -> %d \n",msqid);

    //menu
    //On stock la valeur à calculer dans un champ différent pour permettre au serveur de déterminer le calcul à faire
    switch (opeDemandee)
    {
        case 1:
            printf("Vous avez choisis le calcul des nombres premiers.\n");
            request.operationDemandee = 1;
            request.valeurEnvoyee = valeurDemandee;
            break;
        case 2:
            printf("Vous avez choisis le calcul de la circonference d'un cercle.\n");
            request.operationDemandee = 2;
            request.valeurEnvoyee = valeurDemandee;
            break;
        case 3:
            printf("Vous avez choisis le calcul de la surface d'un cercle.\n");
            request.operationDemandee = 3;
            request.valeurEnvoyee = valeurDemandee;
            break;
        default:
            printf("Vous n'avez pas demandé une opération disponible. \n1 - Calcul des nombres premiers. \n2 - Calcul de la circonférence d'un cercle.\n3 - Calcul de la surface d'un cercle");
            break;
    }

    // utilisation du pid  du processus client
    // pour que le serveur puisse l'identifier de facon unique
    request.PID = getpid(); // pour la reponse
    printf("PID -> %d \n",request.PID);
    printf("msqid -> %d \n",msqid);
    request.mtype = 1;


    //envoi du msg au serveur
    // de taille structure Request - 4 (taille de la variable type, un long est code sur 4 octets )
    if (msgsnd(msqid, &request, sizeof(Request)-sizeof(long), 0) == -1) {
        printf("erreur de send\n");
        perror("msgsnd");
        exit(2);
    }
    
    
    printf("msgsnd envoye\n");

    if (msgrcv(msqid, &resultat, sizeof(Response)-sizeof(long), 1, 1) == -1) {
        printf("erreur de receive\n");
        perror("msgrcv");
        exit(3);
    }
    
    printf("msgrcv reçu\n");
    //affichage du resultat
    printf("Réponse du serveur\n");
  	char *reponseTexte = (char) resultat.result;
    printf("%s  \n", reponseTexte);
    return 0;
}
