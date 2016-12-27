#include "tp4.h"

//Divers
int anneeActuelle (){
	if (year == 0){
		time_t secondes;
		struct tm instant;
		time(&secondes);
		instant = * localtime(&secondes);
		return instant.tm_year + 1900;
	}
	else
		return year;
}

// Déterminer la borne sup de la tranche d'âge d'appartenance
int borneSuperieure(int annee){
 
 	int Actuelle = anneeActuelle();

 	int age = Actuelle - annee;

 	while (age%5 != 0) age++;

 	return age;
}

void viderBuffer()
{
int c;
	while ( (c = getchar()) != '\n' && c != EOF);
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
	newListe -> NbreElements = 0;

	return newListe;
}

//2.2
Tranche * ajoutTranche (Tranche ** racine , int borneSup){
	//printf("Ajout d'une tranche : %d",borneSup);
	//Si l'arbre est vide on initialise la racine
	if (*racine == NULL){
		*racine = nouvelleTranche(borneSup);
		return *racine;
	} 

	Tranche * actuel = *racine, *pere = actuel;
	while (actuel != NULL){

		//Si la borne existe déjà
		if (actuel -> borneSup == borneSup) return actuel;

		//Sinon on parcours
		else if (borneSup < actuel -> borneSup){
			pere = actuel;
			actuel = actuel -> filsG;
		}
		else{
			pere = actuel;
			actuel = actuel -> filsD;
		} 	
	}

	//A ce niveau soit la tranche existe déjà dans l'arbre et on
	//est déjà sorti, soit actuel == NULL

	Tranche * newTranche = nouvelleTranche(borneSup);
	newTranche -> pere = pere;

	if (borneSup < pere -> borneSup){
		pere -> filsG = newTranche;
	} 
	else{
		pere -> filsD = newTranche;
	}

	return newTranche;
}

//2.3
Benevole * insererBen(Tranche ** racine , Benevole * benevole){
	//printf("Ajout du bénévole : %s",benevole -> prenom);
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
	if (actuel -> annee <= benevole -> annee){
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
		if (tmp -> CIN == benevole -> CIN) {
			printf("Désolé, un Bénévole nait la même année et ayant le même CIN est déjà présent dans la base.\n");
			return tmp;
		}
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
//Fonction inutile...
Benevole * chercherBen(Tranche * racine, int CIN, int annee){
	int borneSup = borneSuperieure(annee);

	if (racine == NULL) return NULL;

	Tranche* trancheActuelle=racine;
	
	while (trancheActuelle != NULL && trancheActuelle -> borneSup != borneSup){
		if (borneSup < trancheActuelle -> borneSup) 
			trancheActuelle = trancheActuelle -> filsG;
		else
			trancheActuelle = trancheActuelle -> filsD;
	}

	//Si la tranche n'existe pas on sort
	if (trancheActuelle -> borneSup != borneSup) return NULL;

	Benevole* actuel = trancheActuelle -> liste -> premier;

	while (actuel != NULL){
		if (actuel -> CIN == CIN) return actuel;
		actuel = actuel -> suivant;
	}

	//On ne l'a pas trouve.
	return NULL;
};


//2.5
int supprimerBen(Tranche ** racine , int CIN , int annee){
	int borneSup = borneSuperieure(annee);

	if (*racine == NULL) return 1;

	Tranche* trancheActuelle=*racine;
	
	while (trancheActuelle != NULL && trancheActuelle -> borneSup != borneSup){
		if (borneSup < trancheActuelle -> borneSup) 
			trancheActuelle = trancheActuelle -> filsG;
		else
			trancheActuelle = trancheActuelle -> filsD;
	}

	//Si la tranche n'existe pas on sort
	if (trancheActuelle == NULL){
		printf("Il n'y a pas de bénévoles nait cette année dans l'arbre...\n");
		return 1;
	} 
	if (trancheActuelle -> borneSup != borneSup) {
		printf("Il n'y a pas de bénévoles nait cette année dans l'arbre...\n");
		return 1;
	} 

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
	int a = suppressionBen(actuel,precedent,trancheActuelle);

	//on supprime la tranche si elle est vide !
	
	if (a==0 && trancheActuelle -> liste -> NbreElements == 0){
		a=a+supprimerTranche (racine,trancheActuelle -> borneSup);
		printf("La tranche où était le bénévole étant vide nous l'avons supprimer.\n");
	}
	return a;
}

int suppressionBen(Benevole* actuel, Benevole* precedent, Tranche* trancheActuelle){	
	Benevole* premier = trancheActuelle -> liste -> premier;
	if (actuel == premier){
		//on est au début de la liste
		trancheActuelle -> liste -> premier = premier -> suivant;
		free(actuel);
		trancheActuelle -> liste -> NbreElements --;
		//si la liste résultante est vide, on supprime la classe d'âche de l'arbre
		return 0;
	}
	else if (actuel -> suivant != NULL){
		precedent -> suivant = actuel -> suivant;
		free(actuel);
		trancheActuelle -> liste -> NbreElements --;
		return 0;
	}
	else{//on est à la fin de la liste
		precedent -> suivant = NULL;
		free(actuel);
		trancheActuelle -> liste -> NbreElements --;
		return 0;
	}
	return 1;
}


//2.6
int supprimerTranche (Tranche ** racine , int borneSup){

	if (*racine == NULL) return 1;

	Tranche* actuelle=rechercherTranche(*racine, borneSup);

	//si on ne l'a pas trouvée
	if (actuelle == NULL) return 2;

	if (suppressionListe(actuelle -> liste)==1) return 1;

	//on rétabli l'arbre
	if (suppression(actuelle,racine) == 0) return 0;
	return 1;
}

Tranche* rechercherTranche(Tranche* racine, int borneSup){
	if (racine == NULL) return NULL;

	Tranche* actuelle=racine, *precedent=actuelle;

	while (actuelle != NULL && actuelle -> borneSup != borneSup){
		if (borneSup < actuelle -> borneSup ){
			precedent = actuelle;
			actuelle = actuelle -> filsG;
		} 
		else{
			precedent = actuelle;
			actuelle = actuelle -> filsD;
		}
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
	//liste=NULL;
	free (liste);
	return 0;
}

int suppression(Tranche* actuelle, Tranche ** racine){
	
	//on est obligé de dédoubler pour ne pas avoir de problème avec les mises à jour de racine...
	//si ce n'est pas la racine :
	if (actuelle!=*racine){
		Tranche* pere = actuelle -> pere;

		//Feuille
		if (actuelle -> filsG == NULL && actuelle -> filsD == NULL){
			if (actuelle == pere -> filsG) pere->filsG = NULL;
			else pere -> filsD = NULL;
			//pour éviter les erreurs de segmentations malgré la free mémoire
			actuelle=NULL;
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

			actuelle -> filsD -> pere = pere;
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

			return suppression(succ,racine) + 0;
		}

		return 1;
	}
	else{
		//Si c'est la racine que l'on supprime
		//feuille
		if (actuelle -> filsG == NULL && actuelle -> filsD == NULL){
			free(*racine);
			*racine=NULL;
			return 0;
		}

		//intermédiaire :
		if (actuelle -> filsD == NULL && actuelle -> filsG != NULL){
			Tranche * tmp = actuelle -> filsG;
			free(*racine);
			*racine=tmp;
			return 0;
		}

		if (actuelle -> filsG == NULL && actuelle -> filsD != NULL){
			Tranche * tmp = actuelle -> filsD;
			free(*racine);
			*racine=tmp;
			return 0;
		}

		//Total
		if (actuelle -> filsD != NULL && actuelle -> filsG != NULL){

				Tranche * succ=successeur(actuelle);
				
				//on copie les infos du successeur
				(*racine) -> liste = succ -> liste;
				(*racine) -> borneSup = succ -> borneSup;

				return suppression(succ,racine) + 0;
			}

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

	if (trancheMax == NULL) return NULL;

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

	return listHonneur;
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
int actualiserABR(Tranche ** racine){
	int nbActu=0;


	nbActu=actualisationABR(*racine,racine);

	return nbActu;
}

int actualisationABR(Tranche * racine, Tranche ** ABR){
	if (racine == NULL) return 0;

	int nbActu = 0;

	Benevole* ben = racine -> liste -> premier;
	Benevole* tmp,*suivant,*precedent = ben;
	int borneSup = racine -> borneSup;

	while (ben != NULL){
		if ( borneSuperieure(ben->annee) != borneSup ){
			//si on n'est plus dans la bonne tranche d'âge...
			tmp = copierBenevole(ben);
			suivant = ben -> suivant;
			
			suppressionBen(ben,precedent,racine);
			nbActu++;
			insererBen(ABR,tmp);
			ben=suivant;
		}
		else{
			precedent = ben; 
			ben = ben -> suivant;
		}

	}

	return nbActu + actualisationABR( racine->filsD,ABR ) + actualisationABR( racine->filsG,ABR );

}

int verificationVide(Tranche * racine, Tranche ** ABR){
	if (racine == NULL) return 0;
	int modif = 0;

	if (racine -> liste -> premier == NULL){
		//Si on a un noud avec une liste vide, on supprime le noeud.
		modif = 1;
		free(racine-> liste);
		suppression(racine,ABR);
	}

	return modif + verificationVide(racine -> filsG,ABR) + verificationVide(racine -> filsD,ABR);
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

	if (actuelle != NULL){
		Benevole* ben = actuelle -> liste -> premier;

		while(ben != NULL){
			afficherBenevole(ben);
			ben = ben -> suivant;
		}
	}
	else{
		printf("Aucune tranche ne correspond, elle a pu être supprimée si elle était vide.\n");
	}
	
}

void afficherBenevole (Benevole* ben){
	printf("nom : %s, prenom : %s, sexe : %c, CIN : %d, age : %d\n",ben->nom,ben->prenom,ben->sexe,ben->CIN,anneeActuelle ()-ben->annee);
}

void afficherArbre (Tranche * racine,int niveau){
	//récursif
	if (racine == NULL && niveau == 0) {
		printf("Il n'y a aucune tranche dans l'ABR.\n");
		return;
	}
	if (racine == NULL)
		return;

	afficherArbre(racine->filsG,niveau+1);
	printf("Tranche : %d\n", racine -> borneSup );
	afficherArbre(racine -> filsD,niveau+1);
}


void testing(Tranche *** racineReal){
	Tranche ** racine=*racineReal;
	
	Benevole * ben1=nouveauBen("Florent","Chehab", 200 , 'M', 1980);
	Benevole * ben2=nouveauBen("Timothée","Chehab", 201 , 'M', 1970);
	Benevole * ben3=nouveauBen("Julie","Chehab", 202 , 'F', 1960);
	Benevole * ben4=nouveauBen("Muriel","Chehab", 202 , 'F', 1975);

	insererBen(racine,ben1);
	insererBen(racine,ben2);
	insererBen(racine,ben3);
	insererBen(racine,ben4);
}



//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//CLI

int afficherMenu(){

    int choix = 0;

    printf("\n---Menu---\n\n");

    printf("1.  Détruire et Initialiser un arbre\n");
    printf("2.  Ajouter une Tranche\n");
    printf("3.  Ajouter un bénévole dans une tranche d’âge\n");
    printf("4.  Afficher les tranches d’âge d’un ABR\n");
    printf("5.  Afficher les bénévoles d’une tranche d’âge\n");
    printf("6.  Supprimer un bénévole\n");
    printf("7.  Supprimer une tranche\n");
    printf("8.  Afficher les bénévoles d’honneur\n");
    printf("9.  Afficher le nombre total de bénévoles\n");
    printf("10. Actualiser l’ABR\n");
    printf("11. Afficher le pourcentage de bénévoles d’une tranche d’âge\n");
    printf("12. Quitter\n\n");
    printf("Exprimer votre choix: ");
    scanf("%d", &choix);
    viderBuffer();
    
    return choix;
}

void initialiserCLI(Tranche ** racine){
	if (*racine == NULL){
		printf("L'arbre est déjà vide...\n");
		return;
	}

	viderArbre(racine);
    printf("Tous les espaces mémoires ont bien été supprimées\n");

    printf("L'arbre a été réinitialisé.\n");
}



void ajouterTrancheCLI(){
	printf("#################################################################\n");
	printf("Désolé, cette fonctionnalité n'est pas supportée\n");
	printf("L'ajout de tranche se fait uniquement lors de l'ajout d'un membre\n");
	printf("#################################################################\n");
	printf("\n");
}

void ajouterBenCLI(Tranche ** racine){
	char sexe;
	char nom[TAILLENOM];
	char prenom[TAILLEPRENOM];
	int CIN;
	int annee;

	printf("\n Quel est le nom du nouveau membre ?\n");
	scanf("%s",nom);
	viderBuffer();

	printf("\n Quel est le prénom du nouveau membre ?\n");
	scanf("%s",prenom);
	viderBuffer();
	
	printf("\n Quel est son sexe ?\n");
	scanf("%c",&sexe);
	viderBuffer();

	printf("\n Quel est son numéro de CIN ?\n");
	scanf("%d",&CIN);
	viderBuffer();

	printf("\n Quelle est son année de naissance ?\n");
	scanf("%d",&annee);
	viderBuffer();

	Benevole * newBen=nouveauBen(nom, prenom, CIN, sexe, annee);

	newBen=insererBen(racine,newBen);
	if (newBen != NULL)
		printf("L'insertion c'est bien passée\n");
	else 
		printf("L'insertion en s'est pas bien passée.\n");
	
}

void afficherTrancheCLI(Tranche ** racine){
	if (*racine == NULL){
		printf("Il n'y a aucune tranche d'âge dans l'arbre.\n");
		return;
	}
	printf("\n \n Voici les tranches d'âges qui sont présentes dans notre ABR :\n");
	int niveau = 0;
	afficherArbre(*racine,niveau);
}



void afficherBenCLI(Tranche ** racine){
	int borneSup;

	printf("Quelle tranche d'âge souhaitez-vous afficher ?\n");

	scanf("%d",&borneSup);
	viderBuffer();

	afficherTranche(*racine, borneSup);
}

void supprimerBenCLI(Tranche ** racine){
	if (*racine != NULL){
		int CIN;
		int annee;

		printf("Nous allons chercher à supprimer un membre.\n");

		printf("\n Quel est son numéro de CIN ?\n");
		scanf("%d",&CIN);
		viderBuffer();

		printf("\n Quelle est son année de naissance ?\n");
		scanf("%d",&annee);
		viderBuffer();

		int a=supprimerBen(racine ,  CIN ,  annee);
		if (a==0) printf("La suppression c'est bien passée.\n");
		else printf("La suppression ne s'est pas bien passée.\n");
	}
	else
	{
		printf("Il n'y a aucun bénévole dans l'arbre... On ne peut pas en supprimer.\n");
	}
}


void supprimerTrancheCLI(Tranche ** racine){
	if (*racine == NULL){
		printf("Désolé, il n'y a aucune tranche dans l'arbre...\n");
	}
	else{
		afficherTrancheCLI(racine);
		printf("Quelle tranche d'âge souhaitez-vous supprimer ?\n");
		
		int borneSup;
		scanf("%d",&borneSup);
		viderBuffer();


		int a = supprimerTranche (racine , borneSup);

		if (a==0){
			printf("La suppression c'est bien passée, tous les espaces mémoires concernés ont été vidés.\n");
			printf("Dorénavant :\n");
			afficherTrancheCLI(racine);
		}
		else if (a == 2)
			printf("Cette tranche n'existe pas dans l'arbre...\n");
		else
			printf("Il y a eu une erreure.\n");
	}
	
}


void AfficherBestBenCLI(Tranche ** racine){
	ListBenevoles *tmp= BenDhonneur(*racine);

	if (tmp == NULL){
		printf("Il n'y a aucun bénévole d'honneur...\n");
		return;
	}

	Benevole* actuel = tmp -> premier;



	if (actuel == NULL){
		free(tmp);
		printf("Désolé, il n'y a pas de bénévoles d'honneur...\n");
		return;
	}

	printf("Voici la liste des bénévoles d'honneur.\n");
	while (actuel != NULL){
		afficherBenevole(actuel);
		actuel = actuel -> suivant;
	}

	suppressionListe(tmp);
}


void AfficherNbBenCLI(Tranche ** racine){
	printf("Il y a %d bénévoles dans l'arbre.\n",totalBen(*racine));
}

void ActualiserArbreCLI(Tranche ** racine){

	printf("Dans le programme nous sommes en %d \n",anneeActuelle());
	printf("En quelle année souhaitez-vous déplacer le programme ?\n");
	scanf("%d",&year);
	viderBuffer();
	printf("Le programme a été déplacé en %d \n",anneeActuelle());

	printf("Début de l'acutualisation...\n");


	int nbActu = actualiserABR(racine);

	printf("%d Membres ont changé de classe d'âge.\n",nbActu);
	printf("\n Il faut maintenant vérifier qu'il n'y a pas de classe d'âge vide...\n");

	//Cf printf
	int modification = 1, supprimee=0;
	Tranche * actuelle = *racine;


	while (modification != 0){
		modification = verificationVide(*racine,racine);
		supprimee = supprimee + modification;		
	}

	printf("Tout c'est bien passé, %d classe(s) d'âges vides ont été supprimées.\n",supprimee);
}



void AfficherPercentBenCLI(Tranche ** racine){
	if (*racine == NULL){
		printf("Désolé, il n'y a aucune tranche dans l'arbre...\n");
	}
	else{
		afficherTrancheCLI(racine);
		printf("De quelle tranche d'âge souhaitez-vous afficher le pourcentage?\n");
		
		int borneSup;
		scanf("%d",&borneSup);
		viderBuffer();

		float pourcentage = pourcentageTranche(*racine, borneSup);

		printf("La tranche d'âge %d représente %f %% des bénévoles. \n",borneSup,pourcentage);
	}
}

void viderArbre(Tranche ** racine){
	//on commance par supprimer les fils
	viderInterieur(*racine);
	*racine=NULL;	
}

void viderInterieur(Tranche * racine){
	if (racine -> filsG != NULL)
		viderInterieur(racine-> filsG);
	if (racine -> filsD != NULL)
		viderInterieur(racine-> filsD);

	//on supprime l'élément en lui-même
	if (racine->pere != NULL){
		if (racine == racine->pere->filsG)
			racine -> pere -> filsG = NULL;
		else
			racine -> pere -> filsD = NULL;
	}
	//on supprime les bénévoles :
	suppressionListe(racine -> liste);

	//On libère l'espace mémoire.
	free(racine);
}

int quitterCLI(Tranche ** racine){

	
    printf("Tous les espaces mémoires ont bien été supprimées\n");
    if (*racine == NULL) return 0;

    viderArbre(racine);
    return 0;
}

//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////
//Main
int main(int argc, char const *argv[])
{   
	int state = 1;

	Tranche ** racine;
    *racine = NULL;
    
	testing(&racine);
	printf("##########################################################\n");
   	printf("Un arbre de test a été chargé.\n");
    printf("##########################################################\n");


    while(state){
        switch(afficherMenu()){
            case 1:
            	initialiserCLI(racine);
                break;
            case 2:
            	ajouterTrancheCLI();
                break;
            case 3:
                 ajouterBenCLI(racine);
                break;
            case 4:
                 afficherTrancheCLI(racine);
                break;
            case 5:
                 afficherBenCLI(racine);
                break;
            case 6:
                 supprimerBenCLI(racine);
                break;
            case 7:
                 supprimerTrancheCLI(racine);
                break;
            case 8:
                AfficherBestBenCLI(racine);
                break;
            case 9:
            	AfficherNbBenCLI(racine);
            	break;
            case 10:
                ActualiserArbreCLI(racine);
                break;
            case 11:
                AfficherPercentBenCLI(racine);
                break;
            case 12:
            	state = quitterCLI(racine);
            	break;
            default: 
            	break;
        }
    }

    return 0;
}
