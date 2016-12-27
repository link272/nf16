#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define TAILLENOM 35
#define TAILLEPRENOM 35

typedef struct Benevole Benevole;

struct Benevole {
	char nom[TAILLENOM];
	char prenom[TAILLEPRENOM];
	char sexe;
	int CIN;
	int annee;
	struct Benevole* suivant;
};

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
int borneSuperieure(int annee);

void viderBuffer();


//2.1
Benevole * nouveauBen(char * nom , char * prenom , int CIN , char sexe , int annee);
Tranche * nouvelleTranche (int borneSup);
ListBenevoles * nouvelleListe ();

//2.2
Tranche * ajoutTranche (Tranche ** racine , int borneSup);

//2.3
Benevole * insererBen(Tranche ** racine , Benevole * benevole);


//2.4 
Benevole * chercherBen(Tranche * racine, int CIN, int annee);

//2.5
int supprimerBen(Tranche ** racine , int CIN , int annee);
int suppressionBen(Benevole* actuel, Benevole* precedent,Tranche* trancheActuelle);

//2.6
int supprimerTranche (Tranche ** racine , int borneSup);
Tranche* rechercherTranche(Tranche* racine, int borneSup);
int suppressionListe(ListBenevoles* liste);
int suppression(Tranche* actuelle,Tranche ** racine);
Tranche * successeur(Tranche* actuelle);
Tranche * minimum_ABR(Tranche* actuelle);

//2.7
ListBenevoles * BenDhonneur(Tranche * racine);
Tranche* maximum_ABR(Tranche* actuelle);
Benevole* copierBenevole(Benevole * ben);


//2.8 

//A FAIRE

//2.9
int totalBenTranche (Tranche * racine , int borneSup);
int totalBen(Tranche * racine);
float pourcentageTranche (Tranche * racine , int borneSup);

//2.10
void afficherTranche (Tranche * racine , int borneSup);
void afficherBenevole (Benevole* ben);

//2.11
void afficherArbre (Tranche * racine, int niveau);


void testing(Tranche *** racine);



//suppresion tranche pas op 



//////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////
//Partie CLI
void initialiserCLI(Tranche ** racine);
void ajouterTrancheCLI();
void ajouterBenCLI(Tranche ** racine);
void afficherTrancheCLI(Tranche ** racine);
void afficherBenCLI(Tranche ** racine);
void supprimerBenCLI(Tranche ** racine);
void supprimerTrancheCLI(Tranche ** racine);
void AfficherBestBenCLI(Tranche ** racine);
void AfficherNbBenCLI(Tranche ** racine);
void AfficherPercentBen(Tranche ** racine);
void viderArbre(Tranche ** racine);
void viderInterieur(Tranche * racine);
int quitterSLI(Tranche ** racine);
