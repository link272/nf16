#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define TAILLENOM 35
#define TAILLEPRENOM 35


typedef struct Benevole {
	char nom[TAILLENOM];
	char prenom[TAILLEPRENOM];
	char sexe;
	char CIN;
	int annee;
	struct Benevole* suivant;
} Benevole;

typedef struct ListBenevoles{
	Benevole* premier;
	int NbreElements;
} ListBenevoles;

typedef struct Tranche Tranche;

struct Tranche{
	ListBenevoles* liste;
	int borneSup;
	Tranche* pere;
	Tranche* filsG;
	Tranche* filsD;	
};


//DIVERS

//Détermine l'année actuelle
int anneeActuelle ();

// Déterminer la borne sup de la tranche d'âge d'appartenance
int borneSup(int annee);



//2.1
Benevole * nouveauBen(char * nom , char * prenom , int CIN , char sexe , int annee);
Tranche * nouvelleTranche (int borneSup);
ListBenevoles * nouvelleListe ();

//2.2
Tranche * ajoutTranche (Tranche * racine , int borneSup);

//2.3
Benevole * insererBen(Tranche * racine , Benevole * benevole);


//2.4 
Benevole * chercherBen(Tranche * racine, int CIN, int annee);

//2.5
int supprimerBen(Tranche * racine , int CIN , int annee);

//2.6
int supprimerTranche (Tranche * racine , int borneSup);

//pas forcément initialisé arbre et ajouté tranche dans le menu
//faire gaffe à la racine que l'on ne peut pas modifier 
//passer avec les doubles pointeurs
