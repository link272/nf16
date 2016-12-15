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

//2.2
Tranche * ajoutTranche (Tranche ** racine , int borneSup){

	//Si l'arbre est vide on initialise la racine
	if (racine == NULL){
		*racine = nouvelleTranche(borneSup);
		return *racine;
	} 

	Tranche * actuel = *racine, *precedent = actuel;
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

//2.3
Benevole * insererBen(Tranche * racine , Benevole * benevole){
	
	int borneSup = borneSuperieure(benevole -> annee);
	Tranche * trancheActuelle = ajoutTranche (&racine, borneSup);

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

//2.4
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


//2.5
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

	//Si on l'a trouvé, on le supprime
	return suppressionBen(actuel,precedent,trancheActuelle);
}

int suppressionBen(Benevole* actuel, Benevole* precedent, Tranche* trancheActuelle){	
	Benevole* premier = trancheActuelle -> liste -> premier;
	if (actuel == premier){
		//on est au début de la liste
		trancheActuelle -> liste -> premier = premier -> suivant;
		free(actuel);
		trancheActuelle -> liste -> NbreElements --;
		//si la liste résultante est vide, on supprime la classe d'âche de l'arbre
		if (trancheActuelle -> liste -> premier == NULL) supprimerTranche(trancheActuelle,trancheActuelle->borneSup);
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
	return 1;
}


//2.6
int supprimerTranche (Tranche * racine , int borneSup){

	if (racine == NULL) return 1;

	Tranche* actuelle=rechercherTranche(racine, borneSup);

	//si on ne l'a pas trouvée
	if (actuelle == NULL) return 1;

	if (suppressionListe(actuelle -> liste)==1) return 1;
	

	//on rétabli l'arbre
	if (suppression(actuelle) == 0) return 0;
	return 1;
}

Tranche* rechercherTranche(Tranche* racine, int borneSup){
	if (racine == NULL) return NULL;

	Tranche* actuelle=racine;

	while (actuelle != NULL && actuelle -> borneSup != borneSup){
		if (borneSup < actuelle -> borneSup ) 
			actuelle = actuelle -> filsG;
		else
			actuelle = actuelle -> filsD;
	}

	return actuelle;
}

int suppressionListe(ListBenevoles* liste){
	Benevole* benevole = liste -> premier,*tmp;

	//On supprime les bénévoles
	while (benevole != NULL){
		tmp = benevole;
		benevole = benevole -> suivant;
		free(tmp);
	}

	//On supprime la liste :
	free (liste);
	return 0;
}

int suppression(Tranche* actuelle){
	Tranche* pere = actuelle -> pere;

	//Feuille
	if (actuelle -> filsG == NULL && actuelle -> filsD == NULL){
		if (actuelle == pere -> filsG) pere->filsG =NULL;
		else pere -> filsD == NULL;
		free(actuelle);
		return 0;
	}

	//Intermédiaire
	if (actuelle -> filsD == NULL && actuelle -> filsG != NULL){
		if (actuelle == pere -> filsG) pere -> filsG = actuelle -> filsG;
		else pere -> filsD = actuelle -> filsG;
		actuelle -> filsG -> pere = pere;
		free(actuelle);
		return 0;
	}
	
	if (actuelle -> filsD != NULL && actuelle -> filsG == NULL){
		if (actuelle == pere -> filsG) pere -> filsG = actuelle -> filsD;
		else pere -> filsD = actuelle -> filsD;
		actuelle -> filsG -> pere = pere;
		free(actuelle);
		return 0;
	}

	//total
	if (actuelle -> filsD != NULL && actuelle -> filsG != NULL){

		Tranche * succ=successeur(actuelle);
		
		//on copie les infos du successeur
		actuelle -> liste = succ -> liste;
		actuelle -> borneSup = succ -> borneSup;

		//pas de raccord à faire à priori

		return suppression(succ) + 0;
	}

	return 1;

}

Tranche * successeur(Tranche* actuelle){
	if (actuelle -> filsD != NULL) return minimum_ABR(actuelle -> filsD);

	Tranche* pere = actuelle -> pere,*fils = actuelle;

	while ( pere != NULL && fils == pere -> filsD){
		fils = pere;
		pere = fils -> pere;
	}

	return pere;
}

Tranche* minimum_ABR(Tranche* actuelle){
	Tranche* tmp =actuelle;
	while (tmp -> filsG != NULL) tmp = tmp -> filsG;
	return tmp;
}


//2.7
ListBenevoles * BenDhonneur(Tranche * racine){
	Tranche * trancheMax = maximum_ABR(racine);

	int anneeMAX;
	//on ne peut avoir de tranche vide (voir fonctions précédentes)
	Benevole * ben = trancheMax -> liste -> premier;

	while (ben -> suivant != NULL) ben = ben-> suivant;
	//on récupère l'année du plus âgé
	anneeMAX = ben -> annee;

	//on récupère ceux nait cette année.

	ListBenevoles * listHonneur = nouvelleListe();

	ben = trancheMax -> liste -> premier;
	Benevole* benTMP;
	while (ben != NULL){
		if (ben -> annee == anneeMAX){
			//si on est au début de la liste d'honneur on ajoute le premier élément
			if (listHonneur -> NbreElements == 0){
				listHonneur -> premier = copierBenevole(ben);
				benTMP = listHonneur -> premier;
				listHonneur -> NbreElements ++;
			}
			else{
				benTMP -> suivant = copierBenevole(ben);
				benTMP = benTMP -> suivant;
				listHonneur -> NbreElements ++;
			}
		}
		ben = ben -> suivant;
	}
}

Benevole* copierBenevole(Benevole * ben){
	Benevole * nouveau = nouveauBen(ben -> nom, ben -> prenom, ben -> CIN, ben -> sexe, ben -> annee);
	return nouveau;
}
	
Tranche* maximum_ABR(Tranche* actuelle){
	Tranche* tmp =actuelle;
	while (tmp -> filsD != NULL) tmp = tmp -> filsD;
	return tmp;
}

//2.8
int actualiser(Tranche * racine){
	int nbActu=0;

	int quelleBorne;


	return nbActu;
}


//2.9
int totalBenTranche (Tranche * racine , int borneSup){
	Tranche * actuelle = rechercherTranche (racine,borneSup);

	//la recherche n'a rien donnée
	if (actuelle == NULL){
		printf("Erreure lors de la recherche, pas de tranches correspondante!\n");
		return -1;
	} 

	//sinon
	return actuelle -> liste -> NbreElements;
}

int totalBen(Tranche * racine){
	//en récursif
	if (racine == NULL) return 0;

	return racine -> liste -> NbreElements + totalBen(racine -> filsD) + totalBen(racine -> filsG);
}

float pourcentageTranche (Tranche * racine , int borneSup){
	return (float) 100 * totalBenTranche(racine,borneSup)/totalBen(racine);
}


//2.10
void afficherTranche (Tranche * racine , int borneSup){
	Tranche * actuelle = rechercherTranche (racine,borneSup);

	Benevole* ben = actuelle -> liste -> premier;

	while(ben != NULL){
		afficherBenevole(ben);
		ben = ben -> suivant;
	}
}

void afficherBenevole (Benevole* ben){
	printf("nom : %s, prenom : %s, sexe : %c, CIN : %d, age : %d\n",ben->nom,ben->prenom,ben->sexe,ben->CIN,anneeActuelle ()-ben->annee);
}

void afficherArbre (Tranche * racine){
	//récursif
	if (racine == NULL) return;

	afficherArbre(racine->filsG);
	printf("Borne : %d\n", racine -> borneSup );
	afficherArbre(racine -> filsD);
}



int main(int argc, char const *argv[])
{
	Tranche * racine = NULL;

	Benevole * ben1=nouveauBen("Florent","Chehab", 200 , 'M', 1996);

	//insererBen(racine,ben1);
	return 0;
}
