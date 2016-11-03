#include "TP3.h"



//METHODES COMMUNES------------------------------------------------------------------------------------------------------------------

char* verifierNom(char* nom,char type){
	int taille;
	if (type=='M') taille=tailleNomMarque;
	if (type=='R') taille=tailleNomRayon;
	if (type=='S') taille=tailleNomMagasin; //S pour shop
	while (strlen(nom)>taille){
		printf("Nous sommes désolé, le nom rentré est trop long, merci d'en taper d'une longueur inférieur à %d caractères.\n",taille);
		scanf("%s",nom);
	}

	return nom;
}

int alphaTri(char *var, char *comparatif){
	//verifier la longeur
	int res = 0, i = 0;
	while(res ==0){
		if (var[i] == comparatif[i]){
			i +=1;
		}
		else{
			if(var[i] << comparatif[i])
				res = 1;
			else{
				res = 2;
			}
		}

	}
	return res;
}


//METHODE PRODUIT---------------------------------------------------------------------------------------------------------------------

T_Produit *creerProduit(char *marque, float prix, char qualite, int quantite){
	T_Produit* NouveauProduit=malloc(sizeof(T_Produit));

	marque=verifierNom(marque,'M');
	strcpy(NouveauProduit->marque,marque);
	NouveauProduit->prix=prix;
	NouveauProduit->qualite=qualite;
	NouveauProduit->quantite_en_stock=quantite;
	NouveauProduit->suivant=NULL;

	return NouveauProduit;
}


int chercherProduit(T_Rayon *rayon, char *marque){
	int res = 1;
	T_Produit *actuel;
	actuel = rayon -> premier;
	while(actuel != NULL){
			if(actuel -> marque == marque) res = 0;
			actuel = actuel -> suivant;
		}
		return res;
	}

//Ajout d'un produit dans un rayon
int ajouterProduit(T_Rayon *rayon, T_Produit *produit){
	T_Produit *actuel, *precedent;
	actuel = rayon -> premier;
	while(actuel != NULL){
			precedent = actuel;
			actuel = actuel -> suivant;
		}
	precedent -> suivant = produit;
	
	rayon->nombre_produits += 1;
}


	//renvoie 1 si l'ajout s'est bien passé, 0 sinon; l'ajout se fait en respectant le tri par ordre croissant du prix du produit;
	//on ne doit pas autoriser l'utilisateur à ajouter deux fois la même marque de produit dans un rayon

//Suppresion d'un produit dans un rayon
//renvoie 1 si le retrait s'est bien passé, 0 sinon; on veillera à libérer la mémoire précédemment allouée

int supprimerProduit(T_Rayon *rayon, char* marque_produit){
	//On commence par rechercher l'adresse du produit que l'on veut supprimer.
	T_Produit* Produit=rayon->premier,*ProduitPrecedent; //On récupère l'adresse du premier produit
	while ( (Produit != NULL) && (strcmp(Produit->marque,marque_produit)!=0 )){
		ProduitPrecedent=Produit;
		Produit=Produit->suivant;
	}

	//Si on n'a pas trouvé de produits de la marque
	if (Produit==NULL){
		printf("Nous sommes désolé il n'y a aucun produit de la marque %s dans le rayon %s.\n",rayon->nom_rayon,marque_produit);
		return 0;
	}

	//On s'assure de ne pas perdre des morceaux de rayons...
	ProduitPrecedent->suivant=Produit->suivant;
	free(Produit);
	rayon->nombre_produits -= 1;

	printf("Le produit de la marque %s dans le rayon %s a bien été supprimé. \n",marque_produit,rayon->nom_rayon);
	return 1;
}


//Recherche de produits se situant dans une fourchette de prix entrée par l'utilisateur
void rechercheProduits(T_Magasin *magasin, float prix_min, float prix_max){
	T_Requete *sentinelle, *actuel;
	T_Rayon *rayon=magasin->premier;
	T_Produit *produit=rayon->premier;
	sentinelle -> suivant = NULL;
	sentinelle -> produit = NULL;

	while(rayon!=NULL){
		while(produit !=NULL){
			if((produit->prix >= prix_min) && (produit->prix <= prix_max)){
				if (sentinelle-> suivant == NULL){
					sentinelle -> suivant = creerRequete(produit, rayon);
					sentinelle -> suivant -> precedent = sentinelle;
					actuel = sentinelle -> suivant;
					actuel -> suivant = sentinelle;
				}
				else{
					actuel -> suivant = creerRequete(produit, rayon);
					actuel -> suivant -> precedent = actuel;
					actuel = actuel -> suivant;
					actuel -> suivant = sentinelle;
				}
			}
			produit = produit -> suivant;
		}
		rayon = rayon->suivant;
	}
	trierRequete(sentinelle);
	afficherRequete(sentinelle);
}

void trierRequete(T_Requete *sentinelle){
	//on trie la requete du plus petit prix au plus grand prix
	int changement = 1;
	while (changement){
		changement = 0;
		for(T_Requete *actuel=sentinelle->suivant; actuel->suivant == sentinelle; actuel -> suivant){
			if (actuel -> produit -> prix > actuel -> suivant -> produit -> prix){
				actuel -> precedent -> suivant = actuel -> suivant;
				actuel -> suivant = actuel -> suivant -> suivant;
				actuel -> suivant -> precedent -> suivant = actuel;
				actuel -> suivant -> precedent -> precedent = actuel -> precedent;
				actuel -> precedent = actuel -> suivant -> precedent;
				actuel -> suivant -> precedent = actuel;
				changement = 1;

			}
		}
	}

}

void afficherRequete(T_Requete *sentinelle){
	printf("------------------------------------------------------\n");
    printf("|\tMarque\t|\tPrix\t|\tQualité\t|\tQuantité\t|\tRayon\t|\n");
    printf("------------------------------------------------------\n");
    T_Requete *actuel=sentinelle->suivant;
	while(actuel == sentinelle){
    	printf("|\t%s\t|\t%f\t|\t%c\t|\t%d\t|\t%s\t|\n", actuel -> produit->marque, actuel -> produit->prix, actuel -> produit->qualite, actuel -> produit->quantite_en_stock, actuel->rayon->nom_rayon);
    	actuel = actuel -> suivant;
	}
}

T_Requete *creerRequete(T_Produit *produit, T_Rayon *rayon){
	T_Requete *requete=malloc(sizeof(T_Requete));
	requete->produit = produit;
	requete->rayon = rayon;
	return requete;
}
	//L'affichage se fait sous forme de liste triée par ordre croissant de prix du produit.
	//Vous expliquerez dans votre rapport votre choix de créer des structures spécifiques ou d'utiliser les structures déjà définies
	//pour cette fonction. Vous veillerez à optimiser cette fonction de manière à effectuer le minimum d'opérations possible.
	


//METHODE RAYON------------------------------------------------------------------------------------------------------------------------

T_Rayon *creerRayon(char *nom){
	T_Rayon* NouveauRayon=malloc(sizeof(T_Rayon));

	nom=verifierNom(nom,'R');

	strcpy(NouveauRayon->nom_rayon,nom);
	//On initialise le contenu du rayon
	NouveauRayon->nombre_produits=0;
	NouveauRayon->premier=NULL;
	NouveauRayon->suivant=NULL;

	return NouveauRayon;
}

//Ajout d'un rayon dans un magasin
int ajouterRayon(T_Magasin *magasin, T_Rayon *rayon){
	int res = 0, changement = 1;
	T_Rayon *actuel=magasin->premier, *precedent;
	if(magasin->premier == NULL){
		magasin->premier = rayon;
	}
	else{
		while(actuel != NULL){
			//res = alphaTri(rayon -> nom_rayon, actuel -> nom_rayon);
			if(res == 1){
				rayon -> suivant = precedent -> suivant;
				precedent -> suivant = rayon;
				changement = 0;
				break;
			}
			precedent = actuel;
			actuel = actuel -> suivant;
		}
		if(changement) precedent -> suivant = rayon;
	}
}
	//renvoie 1 si l'ajout s'est bien passé, 0 sinon; l'ajout se fait en respectant le tri par ordre alphabétique sur le nom du
	//rayon; on ne doit pas autoriser l'utilisateur à ajouter deux fois le même rayon

//Affichage de tous les produits d'un rayon
void afficherRayon(T_Rayon *rayon){
    printf("\n");
    printf("------------------------------------------------------\n");
    printf("|\tMarque\t|\tPrix\t|\tQualité\t|\tQuantité\t|\n");
    printf("------------------------------------------------------\n");
    T_Produit *produit=rayon->premier;
    while(produit != NULL){
    printf("|\t%s\t|\t%f\t|\t%c\t|\t%d\t|\n", produit->marque, produit->prix, produit->qualite, produit->quantite_en_stock);
    produit = produit->suivant;
	}
    printf("------------------------------------------------------\n");
}

	//L'affichage se fait sous forme de liste triée sur le prix du produit


//Fusionner deux rayons
//void fusionnerRayons(T_Magasin *magasin);


	//L'utilisateur choisit deux rayons parmi ceux du magasin. On fusionne alors ces deux rayons en un seul dont le nom
	//sera au choix de l'utilisateur et qui contiendra les produits des deux rayons fusionnés triés par ordre croissant de prix.
	//Vous veillerez à optimiser cette fonction de manière à effectuer le minimum d'opérations possible.



T_Rayon *selectionnerRayon(T_Magasin *magasin){
	afficherMagasin(magasin);
	T_Rayon *tmp=malloc(sizeof(T_Rayon)), *res;
	char nom[tailleNomMagasin];
    printf("Entrez un nom de rayon\n");
    scanf("%s", nom);
    tmp = magasin->premier;
    if(strcmp(tmp->nom_rayon,nom)==0){
        res = tmp;
    }
    return res;
}

//Suppression d'un rayon et de tous les produits qu'il contient
//renvoie 1 si la suppression s'est bien passée, 0 sinon; on veillera à libérer la mémoire précédemment allouée


int supprimerRayon(T_Magasin *magasin, char *nom_rayon){
//On commence par rechercher l'adresse du produit que l'on veut supprimer.
	T_Rayon* Rayon=magasin->premier,*RayonPrecedent; //On récupère l'adresse du premier rayon
	
	while ( (Rayon != NULL) && (strcmp(Rayon->nom_rayon,nom_rayon)!=0 )){
		RayonPrecedent=Rayon;
		Rayon=Rayon->suivant;
	}

	//Si on n'a pas trouvé le rayon, on fait un msg d'erreur
	if (Rayon==NULL){
		printf("Nous sommes désolé il n'y a aucun rayon %s dans le magasin %s.\n",nom_rayon,magasin->nom);
		return 0;
	}

	//On s'assure de ne pas perdre des rayons... On bifurque la liste chainée
	RayonPrecedent->suivant=Rayon->suivant;

	//On veille à libérer tout l'espace mémoire prix par les produits du rayons :
	T_Produit* Produit=Rayon->premier,* ProduitSuivant;

	while(Produit != NULL){
		ProduitSuivant=Produit->suivant;
		free(Produit);
		Produit=ProduitSuivant;
	}
	printf("Tous les produits du rayons %s on bien étaient retirés.\n",nom_rayon);

	free(Rayon);

	printf("Le rayon %s a bien été supprimé du magasin %s",nom_rayon,magasin->nom);
	return 1;
}



//METHODE MAGASIN-------------------------------------------------------------------------------------------------------------------------

T_Magasin *creerMagasin(char *nom){
	
	T_Magasin* NouveauMagasin=malloc(sizeof(T_Magasin));

	nom=verifierNom(nom,'S');

	strcpy(NouveauMagasin->nom,nom);
	//On initialise le contenu du magasin.
	NouveauMagasin->premier=NULL;
}

//Affichage de tous les rayons d'un magasin
void afficherMagasin(T_Magasin *magasin){
	printf("\n");
    printf("------------------------------------------------------\n");
    printf("|\tNom du rayon\t|\tNombre de Produit\t|\n");
    printf("------------------------------------------------------\n");
    T_Rayon *rayon=magasin->premier;
	while(rayon != NULL){
		printf("|\t%s\t\t|\t\t%d\t\t|\n", rayon->nom_rayon, rayon->nombre_produits);
		rayon = rayon -> suivant;
	}
	printf("------------------------------------------------------\n");
}

	//L'affichage se fait sous forme de liste triée sur le nom des rayons

	

//PARTIE INTERACTIVE-----------------------------------------------------------------------------------------------------------------------



int afficherMenu(){

    int choix = 0;

    printf("\n---Menu---\n\n");

    printf("1.Créer un magasin\n");
    printf("2.Ajouter un rayon au magasin\n");
    printf("3.Ajouter un produit dans un rayon\n");
    printf("4.Afficher les rayons du magasin\n");
    printf("5.Afficher les produits d'un rayon\n");
    printf("6.Supprimer un produit\n");
    printf("7.Supprimer un rayon\n");
    printf("8.Rechercher un produit par prix\n");
    printf("9.Quitter\n\n");
    scanf("%d", &choix);
    
    return choix;
}

T_Magasin *creerMagasinWrapper(){
	char nom[tailleNomMagasin];
	T_Magasin *magasin;
	printf("Entrer le nom du magasin\n");
    scanf("%s",nom);
    magasin = creerMagasin(nom);
    printf("Le magasin %s est créé\n", magasin ->nom);
    return magasin;
}

void ajouterRayonWrapper(T_Magasin *magasin){
	char nom[tailleNomRayon];
	int nomUnique=0;
	T_Rayon* rayonTMP;
    
    while (nomUnique==0){
	    printf("Entrer le nom du rayon à ajouter au magasin %s\n", magasin -> nom);
	    scanf("%s",nom);
	 
	    verifierNom(nom,'R');
	    //après cette étape le nom est forcément valide.
	 
	    //il nous reste a terster l'unicité

		rayonTMP=magasin->premier;
		nomUnique=1;
		while (rayonTMP != NULL){
			if (strcmp(rayonTMP->nom_rayon,nom)==0){
				printf("Le nom de rayon que vous avez saisi existe déjà.\n");
				nomUnique=0;break;
			}

			rayonTMP=rayonTMP->suivant;
		}
	}

    ajouterRayon(magasin, creerRayon(nom));
    printf("le rayon %s est ajouté au magasin %s\n", nom, magasin -> nom);
}

void ajouterProduitWrapper(T_Magasin *magasin){
	char marque[tailleNomMagasin], qualite;
	float prix;
	int quantite_en_stock;
    printf("Entrer la marque du produit à ajouter\n");
    scanf("%s",marque);
    printf("Entrer la qualité du produit à ajouter\n");
    scanf("%c",qualite);
   	printf("Entrer le prix du produit à ajouter\n");
    scanf("%f",prix);
   	printf("Entrer la quantite du produit à ajouter\n");
    scanf("%f",quantite_en_stock);

    ajouterProduit(selectionnerRayon(magasin), creerProduit(marque, prix, qualite, quantite_en_stock));

}

void afficherMagasinWrapper(T_Magasin *magasin){
	afficherMagasin(magasin);
}

void afficherRayonWrapper(T_Magasin *magasin){
	afficherRayon(selectionnerRayon(magasin));
}

void supprimerProduitWrapper(T_Magasin *magasin){
	char marque[tailleNomMagasin];
	printf("Entrer une marque de produit à supprimer\n");
	scanf("%s", marque);
	supprimerProduit(selectionnerRayon(magasin), marque);
}

void supprimerRayonWrapper(T_Magasin *magasin){
	char nom[tailleNomMagasin];
	printf("Entrer un nom de rayon à supprimer\n");
	scanf("%s", nom);
	supprimerRayon(magasin, nom);
}


void rechercheProduitsWrapper(T_Magasin *magasin){
	float prix_min, prix_max;
	printf("Entrer une fouchette de prix\n");
	printf("Prix minimum\n");
	scanf("%f", prix_min);
	printf("Prix maximum\n");
	scanf("%f", prix_max);
	rechercheProduits(magasin, prix_min, prix_max);
}


void main(){
	T_Magasin *magasin;
	int state = 1;
    while(state){
        switch(afficherMenu()){
            case 1:
                magasin = creerMagasinWrapper();
                break;
            case 2:
            	ajouterRayonWrapper(magasin);
                break;
            case 3:
                ajouterProduitWrapper(magasin);
                break;
            case 4:
                afficherMagasinWrapper(magasin);
                break;
            case 5:
                afficherRayonWrapper(magasin);
                break;
            case 6:
                supprimerProduitWrapper(magasin);
                break;
            case 7:
                supprimerRayonWrapper(magasin);
                break;
            case 8:
                rechercheProduitsWrapper(magasin);
                break;
            case 9:
                state = 0;
                break;
        }
    }
}
