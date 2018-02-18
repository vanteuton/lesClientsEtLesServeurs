

#include <sys/msg.h>
#include <sys/ipc.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>


//_______________________________________________________________________________________________________________
// CONSTANTES CONSTANTES CONSTANTES CONSTANTES CONSTANTES CONSTANTES CONSTANTES CONSTANTES CONSTANTES CONSTANTES
//_______________________________________________________________________________________________________________

#define PI  3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111

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
    char* result;
};


typedef struct Response Response;

//_______________________________________________________________________________________________________________
// FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS FONCTIONS
//_______________________________________________________________________________________________________________


float estPremier (float nombre)
{
    int intNombre = (int) nombre;
    double doubleNombre = (double) nombre;
    double nombreLimite = sqrt(doubleNombre);
    float reponse = 1;

    for(int i = 3; i < nombreLimite; i =+ 2)
    {
        if (intNombre % i == 0){
            reponse =  0;
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
    Request request;
    Response resultat;
    char buffer[40];
    resultat.result = malloc(sizeof(char[40]));
  	printf("SERVEUR SERVEUR SERVEUR SERVEUR SERVEUR SERVEUR SERVEUR SERVEUR \n");

    //ouverture la file de msg CLE, la cree si elle nexiste pas
    if ((msqid = msgget((key_t) CLE, IPC_CREAT|0750)) == -1) {
        // affichage de l'erreur provenant du dernier appel system
        perror("msgget");
        exit(1);
    }
    
    printf("msqid -> %d \n",msqid);
  

    while (1) {
      	if (msgrcv(msqid, &request, sizeof(request)-sizeof(long), 1 , 0) == -1) {
          printf("erreur de receive\n");
          perror("msgrcv");
          exit(3);
    		}
        switch (request.operationDemandee)
        {
            case 1:
                printf("cas 1 demandé \n");
                if(estPremier(request.valeurEnvoyee)) {
                    snprintf(buffer, sizeof buffer, "Le nombre est premier");
                    resultat.result = buffer;
                }
                else {
                    snprintf(buffer, sizeof buffer, "Le nombre n'est pas premier");
                    resultat.result = buffer;
                }
          			printf("%s",resultat.result);
      					resultat.mtype = 1;	
                //envoi du msg resultat au client identifié par son pid
                if (msgsnd(msqid, &resultat, sizeof(Response)-sizeof(long), 1) == -1) {
                    perror("msgsnd");
                    exit(3);
                }
          			printf("msg envoyé\n");
                break;
            case 2:
                printf("cas 2 demandé \n");
                snprintf(buffer, sizeof buffer, "Le perimetre du disque est %f", perimetreCercle(request.valeurEnvoyee));
                resultat.result = buffer;
          			printf("%s",resultat.result);
      					resultat.mtype = 1;	
                //envoi du msg resultat au client identifié par son pid
                if (msgsnd(msqid, &resultat, sizeof(Response)-sizeof(long), 1) == -1) {
                    perror("msgsnd");
                    exit(3);
                }
          			printf("msg envoyé\n");
                break;
            case 3:
                printf("cas 3 demandé \n");
                snprintf(buffer, sizeof buffer, "L'air du disque est %f", aireDisque(request.valeurEnvoyee));
                resultat.result = buffer;
          			printf("%s",resultat.result);
      					resultat.mtype = 1;	
                //envoi du msg resultat au client identifié par son pid
                if (msgsnd(msqid, &resultat, sizeof(Response)-sizeof(long), 1) == -1) {
                    perror("msgsnd");
                    exit(3);
                }
          			printf("msg envoyé\n");
                break;
            default:
                break;
        }

    }
    return 0;
}
