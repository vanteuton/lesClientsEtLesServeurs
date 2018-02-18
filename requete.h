#include <unistd.h>

#ifndef REQUETE_H_
#define REQUETE_H_

#define CLE 160
#define PI  3.14159265358979323846264338327950288419716939937510582097494459230781640628620899862803482534211706798214808651328230664709384460955058223172535940812848111

// structure message pour la requete
struct Request
{
    long type;
    int choix;
    int nbPremier;   	 //pour le nombre premier
    float circCercle;    //pour le calcul circonférence du cercle
    float surfCercle;    //pour le calcul surface du cercle
    pid_t PID;
};
typedef struct Request Request;

// structure message pour la reponse
struct Response
{
    long type;
    int resNbPremier;     //resultat nb premier
    float resCercle;      //resultat pour calcul circonférence du cercle
    float resSurfCercle;  //resultat pour le calcul surface du cercle
};

typedef struct Response Response;


#endif /* REQUETE_H_ */
