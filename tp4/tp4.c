#include "tp4.h"

//Divers
int anneeActuelle (){
	time_t secondes;
	struct tm instant;
	time(&secondes);
	instant = * localtime(&secondes);
	return instant.tm_year + 1900;
}

// Déterminer la borne sup de la tranche d'âge d'appartenance
int borneSuperieure(int annee){
 
 	int Actuelle = anneeActuelle();

 	int age = Actuelle - annee;

 	while (age%5 != 0) age++;

 	return age;
}




//2.1
Benevole * nouveauBen(char * nom , char * prenom , int CIN , char sexe , int annee){
	
	Benevole* newBenevole= (Benevole*)malloc(sizeof(Benevole));

	strcpy(newBenevole -> nom,nom);
	strcpy(newBenevole -> prenom,prenom);
	newBenevole -> CIN = CIN;
	newBenevole -> sexe = sexe;
	newBenevole -> annee = annee;
	newBenevole -> suivant = NULL;

	return newBenevole;
}

Tranche * nouvelleTranche (int borneSup){

	Tranche* newTranche = (Tranche *) malloc(sizeof(Tranche));

	newTranche -> liste = nouvelleListe();
	newTranche -> borneSup = borneSup;
	newTranche -> pere = NULL;
	newTranche -> filsG = NULL;
	newTranche -> filsD = NULL;

	return newTranche;
}

ListBenevoles * nouvelleListe (){

	ListBenevoles* newListe = (ListBenevoles*) malloc(sizeof(ListBenevoles));

	newListe -> premier = NULL;
	int NbreElements = 0;

	return newListe;
}


Tranche * ajoutTranche (Tranche * racine , int borneSup){

	//Si l'arbre est vide on initialise la racine
	if (racine == NULL){
		racine = nouvelleTranche(borneSup);
		return racine;
	} 

	Tranche * actuel = racine, *precedent = actuel;
	while (actuel != NULL){

		//Si la borne existe déjà
		if (actuel -> borneSup == borneSup) return actuel;

		//Sinon on parcours
		if (actuel -> borneSup > borneSup){
			precedent = actuel;
			actuel = actuel -> filsG;
		}

		if (actuel -> borneSup < borneSup){
			precedent = actuel;
			actuel = actuel -> filsD;
		} 	
	}

	//A ce niveau soit la tranche existe déjà dans l'arbre et on
	//est déjà sorti, soit actuel == NULL

	Tranche * newTranche = nouvelleTranche(borneSup);
	newTranche -> pere = precedent;

	if (precedent -> borneSup < borneSup) 
		precedent -> filsG = newTranche;
	else 
		precedent -> filsD = newTranche;

	return newTranche;
}


Benevole * insererBen(Tranche * racine , Benevole * benevole){
	
	int borneSup = borneSuperieure(benevole -> annee);
	Tranche * trancheActuelle = ajoutTranche (racine, borneSup);

	Benevole* actuel=trancheActuelle -> liste -> premier, *precedent;
	
	//Si la liste dans la tranche est vide, on l'initialise 
	if (actuel == NULL){
		trancheActuelle -> liste -> premier = benevole;
		trancheActuelle -> liste -> NbreElements ++;
		return benevole;
	}

	//si il faut ajouter au début de la liste
	if (actuel -> annee < benevole -> annee){
		benevole -> suivant = trancheActuelle -> liste -> premier;
		trancheActuelle -> liste -> premier = benevole;
		trancheActuelle -> liste -> NbreElements ++;
		return benevole;
	}

	while (actuel != NULL && actuel -> annee > benevole -> annee ){
		precedent = actuel;
		actuel = actuel -> suivant;
	}

	//Vérif existance 
	Benevole* tmp= actuel;
	while (tmp != NULL && tmp -> annee == benevole -> annee){
		if (tmp -> CIN == benevole -> CIN) return tmp;
		tmp = tmp -> suivant;
	}


	if (actuel == NULL){
		//ajout à la fin de la liste
		precedent -> suivant = benevole;
		trancheActuelle -> liste -> NbreElements ++;
		return benevole;
	}
	else{
		//Ajout au milieu
		precedent -> suivant = benevole;
		benevole -> suivant = actuel;
		trancheActuelle -> liste -> NbreElements ++;
		return benevole;
	}
}

Benevole * chercherBen(Tranche * racine, int CIN, int annee){
	int borneSup = borneSuperieure(annee);

	if (racine == NULL) return NULL;

	Tranche* trancheActuelle=racine;
	int trouve = 0;
	while (trancheActuelle -> borneSup != borneSup && !trouve){
		if (trancheActuelle -> borneSup == borneSup) trouve = 1;
		if (borneSup < trancheActuelle -> borneSup) 
			trancheActuelle = trancheActuelle -> filsG;
		else
			trancheActuelle = trancheActuelle -> filsD;
	}

	//Si la tranche n'existe pas on sort
	if (trouve == 0) return NULL;

	Benevole* actuel = trancheActuelle -> liste -> premier;

	while (actuel != NULL){
		if (actuel -> CIN == CIN) return actuel;
		actuel = actuel -> suivant;
	}

	//On ne l'a pas trouve.
	return NULL;
};


int supprimerBen(Tranche * racine , int CIN , int annee){
	int borneSup = borneSuperieure(annee);

	if (racine == NULL) return 1;

	Tranche* trancheActuelle=racine;
	int trouve = 0;
	while (trancheActuelle -> borneSup != borneSup && !trouve){
		if (trancheActuelle -> borneSup == borneSup) trouve = 1;
		if (borneSup < trancheActuelle -> borneSup) 
			trancheActuelle = trancheActuelle -> filsG;
		else
			trancheActuelle = trancheActuelle -> filsD;
	}

	//Si la tranche n'existe pas on sort
	if (trouve == 0) return 1;

	Benevole* premier = trancheActuelle -> liste -> premier, *actuel = premier, *precedent = actuel;

	//si la liste est vide
	if (premier == NULL) return 1;

	while (actuel != NULL){
		if (actuel -> CIN == CIN) break; //on sort du while
		precedent = actuel;
		actuel = actuel -> suivant;
	}

	//On ne l'a pas trouve.
	if (actuel == NULL) return 1;

	if (actuel == premier){
		//on est au début de la liste
		trancheActuelle -> liste -> premier = premier -> suivant;
		free(actuel);
		trancheActuelle -> liste -> NbreElements --;
		return 0;
	}
	else if (actuel -> suivant != NULL){
		precedent -> suivant = actuel -> suivant;
		free(actuel);
		trancheActuelle -> liste -> NbreElements --;
		return 0;
	}
	else{//on est à la fin de la liste
		precedent -> suivant == NULL;
		free(actuel);
		trancheActuelle -> liste -> NbreElements --;
		return 0;
	}
	
}

int main(int argc, char const *argv[])
{
	/* code */
	return 0;
}
