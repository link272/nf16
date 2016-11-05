#include "TP3.h"



//METHODES COMMUNES------------------------------------------------------------------------------------------------------------------

char* verifierNom(char* nom,char type){
	int taille;
	if (type=='M') taille=tailleNomMarque;
	if (type=='R') taille=tailleNomRayon;
	if (type=='S') taille=tailleNomMagasin; //S pour shop7
	if (type=='P') taille=taillePrix;
	while (strlen(nom)>taille){
		printf("Nous sommes désolé, le nom ou le prix rentré est trop long, merci d'en taper d'une longueur inférieur à %d caractères.\n",taille);
		scanf("%s",nom);
		viderBuffer();
	}

	return nom;
}

//Supprimer une fois dans tp3.h
T_Produit *chercherProduit(T_Rayon *rayon, char *marque);
//
 
 //NE PAS SUPPRIMER ça !!
void verifierMarqueProduit(T_Rayon* rayon,char* marque){
    while(chercherProduit(rayon,marque)!=NULL){
        printf("La marque que vous avez tappée existe déjà dans ce rayon, merci de tapper une autre marque\n");
        printf("Entrer la marque du produit à ajouter\n");
        scanf("%s", marque);
        viderBuffer();
        verifierNom(marque,'M');
    }
}



//Fonction pour vérifer la saisie de qualité.
void checkQualite(char* q){
	int test=0;
	if (*q=='A') test++;
	if (*q=='B') test++;
	if (*q=='C') test++;

	while(test==0){
		viderBuffer();
		printf("La lettre saisie n'a pas étée reconnue.\n");
		printf("Merci de saisir comme qualité la lettre A, B ou C.\n");
		scanf("%c",q);
		
		test =0;
		if (*q=='A') test++;
		if (*q=='B') test++;
		if (*q=='C') test++;
	}
}

int strCompareAlpha(char* str1,char *str2){
	//retourne -1 si str1 est avant str2; 0 si les deux sont égaux; 1 sinon

	if (strcmp(str1,str2)==0) return 0;

	
	int dist='a'-'A';

	char c1,c2;

	int i;
	int n,pluspetit;
	if (strlen(str1)<strlen(str2)){
		n=strlen(str1);pluspetit=1;
	} 
	else {
		n=strlen(str2);pluspetit=2;
	}

	for (i = 0; i < n; ++i)
	{
		c1=*(str1+i);
		c2=*(str2+i);

		//si c'est des lettes on passe en majuscule.

		if (c1>='a' && c1<='z') c1=c1-dist;
		if (c2>='a' && c2<='z') c2=c2-dist;

		//
		if (c1<c2) return -1;
		if (c2<c1) return 1;
	}

	//Si tous le début est pareil on fait un dernier test
	if (pluspetit==1) return -1;
	else return 1;
}

void viderBuffer()
{
int c;
	while ((c = getchar()) != '\n' && c != EOF);
}


void afficherPrix(float prix){
	// partie entière
	int nb = round(prix);
	// partie décimal
	int decimales=round((prix-nb)*100);

	printf("%d€%d",nb,decimales);
}

int NombreProduitsMagasin(T_Magasin *magasin){
	T_Rayon* rayon=magasin->premier;
	int total;
	while (rayon != NULL){
		total=total + rayon->nombre_produits;
		rayon=rayon->suivant;
	}
	return total;
}

//Blindage de la saisie du prix
float saisiePrix(){
	float prix;
	int test=0, nbsigne=0;
	char str[taillePrix];
	int i=0;

	while (test==0 || nbsigne>1 || i==0){
		nbsigne=0;
		if (test!=0) printf("\nIl y a eu une erreure veuillez recommencer votre saisie.\n");
		printf("Entrer le prix du produit sous la forme 1.12 pour 1€12\n");
		scanf("%s",str);
		viderBuffer();

		verifierNom(str,'P');

		i=0;
		while (str[i]!='\0'){
			if ( str[i]<'0' || str[i] >'9'){
				if (str[i]=='.') nbsigne++;
				else {nbsigne=2;break;} //Caractère impossible rentré
			} 
			i++;
		}
		test++;
	}
	
	prix=(float) atof(str);
	printf("\nLe prix saisie est :\n");
	afficherPrix(prix);
	printf("\n \n");
	return prix ;
}


int saisieQuantite(){
	int q;
	int test=0, nbsigne=0;
	char str[tailleQuantite]="-1";
	int i=0;

	while (test==0 || nbsigne>0 || i==0 || atoi(str)<1){
		nbsigne=0;
		if (test!=0) printf("\nIl y a eu une erreure veuillez recommencer votre saisie.\n");
		printf("Entrer la quantite du produit.\n");
		printf("Merci de saisir un nombre entier différent de 0.\n");
		scanf("%s",str);
		viderBuffer();

		verifierNom(str,'Q');

		i=0;
		while (str[i]!='\0'){
			if ( str[i]<'0' || str[i] >'9'){
				nbsigne=2;break; //Caractère impossible rentré
			} 
			i++;
		}
		test++;
	}
		

	return  atoi(str);
}

char* nouveauNomRayon(T_Magasin *magasin, char* nom){
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
	return nom;
}
 

//METHODE PRODUIT---------------------------------------------------------------------------------------------------------------------


//Initialistation d'un produit
T_Produit *creerProduit(char *marque, float prix, char qualite, int quantite){
	T_Produit* produit=malloc(sizeof(T_Produit));
	strcpy(produit->marque,marque);
	produit->prix=prix;
	produit->qualite=qualite;
	produit->quantite_en_stock=quantite;
	produit->suivant=NULL;
	produit->rayon=NULL;
	return produit;
}

//recherche d'un produit
T_Produit *chercherProduit(T_Rayon *rayon, char *marque){
	T_Produit *produit=rayon->premier;
	while(produit != NULL){
			if(strcmp(produit -> marque, marque) == 0){
				return produit;
			}
			produit = produit -> suivant;
	}
	return NULL;
}

//Ajout d'un produit dans un rayon
int ajouterProduit(T_Rayon *rayon, T_Produit *produit){
	T_Produit *actuel = rayon -> premier;
	//si le rayon est vide on ajoute notre produit directement
	if(rayon -> premier == NULL){
		rayon -> premier = produit;
		rayon->nombre_produits ++;
		return 1;
	}
	//Sinon on regarde où on le place
	else{
		if (produit->prix<=actuel->prix){
			produit->suivant=actuel;
			rayon->premier=produit;
			rayon->nombre_produits ++;
			return 1;
		}
		while(actuel->suivant != NULL){
			if ( (actuel->prix <= produit->prix) && ( (actuel->suivant)->prix >= produit->prix) ){
				produit->suivant=actuel->suivant;
				actuel->suivant=produit;
				rayon->nombre_produits ++;
				return 1;
			}
			actuel = actuel -> suivant;
		}

		actuel->suivant = produit;
		produit->suivant = NULL;
		rayon->nombre_produits ++;
		return 1;
	}
	return 0;	
}

//renvoie 1 si l'ajout s'est bien passé, 0 sinon; l'ajout se fait en respectant le tri par ordre croissant du prix du produit;
//on ne doit pas autoriser l'utilisateur à ajouter deux fois la même marque de produit dans un rayon
//Suppresion d'un produit dans un rayon
//renvoie 1 si le retrait s'est bien passé, 0 sinon; on veillera à libérer la mémoire précédemment allouée

int supprimerProduit(T_Rayon *rayon, char* marque_produit){
	//On commence par rechercher l'adresse du produit que l'on veut supprimer.
	T_Produit* produit=rayon->premier,*precedent=rayon->premier; //On récupère l'adresse du premier produit
	while ( (produit != NULL) && (strcmp(produit->marque,marque_produit)!=0 )){
		precedent=produit;
		produit=produit->suivant;
	}

	//On s'assure de ne pas perdre des morceaux de rayons...
	if (precedent==produit) rayon->premier=produit->suivant;
	else precedent->suivant=produit->suivant;
	
	free(produit);
	rayon->nombre_produits--;

	printf("Le produit de la marque %s dans le rayon %s a bien été supprimé. \n",marque_produit,rayon->nom_rayon);
	return 1;
}


// METHODE REQUETE --------------------------------------------------------------------------------------------------------------------------------

//Recherche de produits se situant dans une fourchette de prix entrée par l'utilisateur
void rechercheProduits(T_Magasin *magasin, float prix_min, float prix_max){
	//on crée un rayon temporaire qui stockera les produits trouvés
	//Les produits stockés sont automatiquement triés par ordre de prix, lors de leur ajout.
	T_Rayon* rayonTMP=malloc(sizeof(T_Rayon));
	T_Rayon* rayon=magasin->premier;
	T_Produit* produit;
	T_Produit* produitTMP;

	while(rayon!=NULL){
		//On parcours les rayons
		produit=rayon->premier;

		while(produit!=NULL){
			//on récupère les produits
			if (produit->prix >= prix_min){
				if (produit->prix <= prix_max){
					produitTMP=creerProduit(produit->marque,produit->prix,produit->qualite,produit->quantite_en_stock);
					produitTMP->rayon=rayon;
					ajouterProduit(rayonTMP,produitTMP);
				}
				else{ produit=NULL;break;break;}
			} 
			produit=produit->suivant;
		}

		rayon=rayon->suivant;
	}


	//on affiche les produits
	if (rayonTMP->premier != NULL) afficherRechercheProduit(rayonTMP);
	else printf("Nous sommes désolés, aucun produit ne correspons à votre recherche.\n");

	//on libère l'espace mémoire.
	T_Produit* precedent;
	produit=rayonTMP->premier;

	while (produit != NULL){
		precedent = produit;
		free(produit);
		produit=produit->suivant;
	}
	free(rayonTMP);
}

void afficherRechercheProduit(T_Rayon *rayon){

	printf("-----------------------------------------------------------------------------------------------------------------\n");
    printf("|\tMarque\t\t|\tPrix\t|\tQualité\t|\tQuantité\t|\tRayon\t\t|\n");
    printf("-----------------------------------------------------------------------------------------------------------------\n");
    T_Produit *actuel=rayon->premier;

	while(actuel != NULL){
    	printf("|\t%12s\t|\t", actuel ->marque);
    	afficherPrix(actuel->prix);
    	printf("\t|\t%c\t|\t%d\t\t|\t%12s\t|\n", actuel ->qualite, actuel ->quantite_en_stock, actuel->rayon->nom_rayon);

    	actuel = actuel -> suivant;
	}
	printf("-----------------------------------------------------------------------------------------------------------------\n");

}


	//L'affichage se fait sous forme de liste triée par ordre croissant de prix du produit.
	//Vous expliquerez dans votre rapport votre choix de créer des structures spécifiques ou d'utiliser les structures déjà définies
	//pour cette fonction. Vous veillerez à optimiser cette fonction de manière à effectuer le minimum d'opérations possible.
	


//METHODE RAYON------------------------------------------------------------------------------------------------------------------------

T_Rayon *creerRayon(char *nom){
	T_Rayon* rayon=malloc(sizeof(T_Rayon));
	strcpy(rayon->nom_rayon,nom);
	rayon->nombre_produits=0;
	rayon->premier=NULL;
	rayon->suivant=NULL;
	return rayon;
}

//Ajout d'un rayon dans un magasin avec tri
int ajouterRayon(T_Magasin *magasin, T_Rayon *rayon){
	T_Rayon *actuel=magasin->premier;
	//Si le magasin est vide on ajoute directe
	if(magasin->premier == NULL){
		magasin->premier = rayon;
		return 1;
	}
	else{
		//Sinon on regarde où il faut mettre le rayon

		if (strCompareAlpha(rayon->nom_rayon,actuel->nom_rayon)==-1){
			rayon->suivant=actuel;
			magasin->premier=rayon;
			return 1;
		}

		while(actuel->suivant!=NULL){
			//si on est au "milieu"
			if (strCompareAlpha(rayon->nom_rayon,actuel->nom_rayon)==-1 && strCompareAlpha(rayon->nom_rayon,(actuel->suivant)->nom_rayon)==1){
				rayon->suivant=actuel->suivant;
				actuel->suivant=rayon;
				return 1;
			}
			actuel = actuel -> suivant;
		}
		actuel -> suivant = rayon;
		rayon->suivant = NULL;
		return 1;
	}
	return 0;
}
	//renvoie 1 si l'ajout s'est bien passé, 0 sinon; l'ajout se fait en respectant le tri par ordre alphabétique sur le nom du
	//rayon; on ne doit pas autoriser l'utilisateur à ajouter deux fois le même rayon

//Affichage de tous les produits d'un rayon
void afficherRayon(T_Rayon *rayon){
    printf("\n");
    printf("---------------------------------------------------------------------------------\n");
    printf("|\tMarque\t\t|\tPrix\t|\tQualité\t|\tQuantité|\n");
    printf("---------------------------------------------------------------------------------\n");
    T_Produit *produit = rayon->premier;
    while(produit != NULL){
    	printf("|\t%12s\t|\t", produit->marque);
    	afficherPrix(produit->prix);
    	printf("\t|\t%c\t|\t%d\t|\n",produit->qualite, produit->quantite_en_stock);
    	produit = produit->suivant;
	}
    printf("---------------------------------------------------------------------------------\n");
}

	//L'affichage se fait sous forme de liste triée sur le prix du produit


//Fusionner deux rayons
//void fusionnerRayons(T_Magasin *magasin);


	//L'utilisateur choisit deux rayons parmi ceux du magasin. On fusionne alors ces deux rayons en un seul dont le nom
	//sera au choix de l'utilisateur et qui contiendra les produits des deux rayons fusionnés triés par ordre croissant de prix.
	//Vous veillerez à optimiser cette fonction de manière à effectuer le minimum d'opérations possible.

T_Rayon *selectionnerRayon(T_Magasin *magasin){
	afficherMagasin(magasin);
	T_Rayon *actuel;
	char nom[tailleNomRayon];

	int test=0; //On verifie l'existence du rayon rentré
	while (test==0){
		printf("Entrez un nom de rayon\n");
	    scanf("%s",nom);
	    viderBuffer();

	    actuel = magasin->premier;
	    while (actuel!= NULL){
	    	if(strcmp(actuel->nom_rayon,nom)==0){
	        return actuel;
	    	}
	    	actuel=actuel->suivant;
	    }
	    printf("Il n'existe aucun rayon de ce nom dans le magasin %s.\n \n",magasin->nom);
	    printf("Pour rappel voici la liste des rayons du magasin %s.\n",magasin->nom);
	    afficherMagasin(magasin);
	}
    
}

//Suppression d'un rayon et de tous les produits qu'il contient
//renvoie 1 si la suppression s'est bien passée, 0 sinon; on veillera à libérer la mémoire précédemment allouée


int supprimerRayon(T_Magasin *magasin, char *nom_rayon, int suppProduit){
//On commence par rechercher l'adresse du rayon que l'on veut supprimer.
	T_Rayon* Rayon=magasin->premier,*RayonPrecedent=magasin->premier; //On récupère l'adresse du premier rayon
	
	while ( (Rayon != NULL) && (strcmp(Rayon->nom_rayon,nom_rayon)!=0 )){
		RayonPrecedent=Rayon;
		Rayon=Rayon->suivant;
	}

	//On veille à libérer tout l'espace mémoire pris par les produits du rayons :
	//Que si c'est demandé
	if (suppProduit==1){
		T_Produit* Produit=Rayon->premier,* ProduitSuivant;

		while(Produit != NULL){
			ProduitSuivant=Produit->suivant;
			free(Produit);
			Produit=ProduitSuivant;
		}
	}
	

	//On s'assure de ne pas perdre des rayons... On bifurque la liste chainée
	
	if (Rayon==RayonPrecedent) magasin->premier=Rayon->suivant;
	else RayonPrecedent->suivant=Rayon->suivant;

	free(Rayon);
	return 1;
}



//METHODE MAGASIN-------------------------------------------------------------------------------------------------------------------------

T_Magasin *creerMagasin(char *nom){
	T_Magasin* magasin=malloc(sizeof(T_Magasin));
	strcpy(magasin->nom,nom);
	magasin->premier=NULL;
	return magasin;
}

//Affichage de tous les rayons d'un magasin
void afficherMagasin(T_Magasin *magasin){
	printf("\n");
    printf("-----------------------------------------------------------------\n");
    printf("|\tNom du rayon\t\t|\tNombre de Produit\t|\n");
    printf("-----------------------------------------------------------------\n");
    T_Rayon *rayon=magasin->premier;
	while(rayon != NULL){
		printf("|\t%12s\t\t|\t\t%d\t\t|\n", rayon->nom_rayon, rayon->nombre_produits);
		rayon = rayon -> suivant;
	}
	printf("-----------------------------------------------------------------\n");
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
    printf("9.Fusionner deux Rayons\n");
    printf("10.Quitter\n\n");
    printf("Exprimer votre choix: ");
    scanf("%d", &choix);
    viderBuffer();
    
    return choix;
}

T_Magasin *creerMagasinWrapper(){
	char nom[tailleNomMagasin];
	T_Magasin *magasin;
	printf("Entrer le nom du magasin\n");
    scanf("%s",nom);
    viderBuffer();
    verifierNom(nom,'S');
    magasin = creerMagasin(nom);
    printf("Le magasin %s est créée\n", magasin ->nom);
    return magasin;
}

void ajouterRayonWrapper(T_Magasin *magasin){
	char nom[tailleNomRayon];
	  
	nouveauNomRayon(magasin,nom);

    ajouterRayon(magasin, creerRayon(nom));
    printf("le rayon %s est ajouté au magasin %s\n", nom, magasin -> nom);
}

void ajouterProduitWrapper(T_Magasin *magasin){
    char marque[tailleNomMagasin], qualite;
    float prix;
    int quantite_en_stock;
    T_Produit *produit;
    T_Rayon *rayon;
    //on commence par choisir le rayon pour pouvoir vérifier que la marque n'est pas déjà présente
    rayon = selectionnerRayon(magasin);
 
    printf("Entrer la marque du produit à ajouter\n");
    scanf("%s", marque);
    viderBuffer();
    verifierNom(marque,'M');
    verifierMarqueProduit(rayon,marque);
 
    printf("Entrer la qualité du produit à ajouter (A, B ou C)\n");
    scanf("%c", &qualite);
    checkQualite(&qualite);
 
    prix=saisiePrix();

    quantite_en_stock=saisieQuantite();
 
    
    produit = creerProduit(marque, prix, qualite, quantite_en_stock);
 
    
   int a= ajouterProduit(rayon, produit);
   if (a==1) printf("Le produit a bien été ajouté.\n");
   else printf("Il y a eu une erreure.\n");
}

void afficherMagasinWrapper(T_Magasin *magasin){
	afficherMagasin(magasin);
}

void afficherRayonWrapper(T_Magasin *magasin){
	afficherRayon(selectionnerRayon(magasin));
}

void supprimerProduitWrapper(T_Magasin *magasin){
	//on vérifie que le mgasin n'est pas vide de produit.
	if(NombreProduitsMagasin(magasin) ==0) printf("Le magasin est actuellement vide (aucun produit). Veuillez d'abord créer un produit...\n");
	else{
		char marque[tailleNomMarque];
		T_Rayon* rayon;
		int test=0;

		//on vérifie qu'on ne sélectionne pas un rayon vide
		while (test==0 || rayon->premier==NULL){
			if (test != 0) printf("Désolé ce rayon est vide, merci dans choisir un autre.\n \n");
			printf("Dans quel rayon se trouve le produit à supprimer ?\n");
			rayon=selectionnerRayon(magasin);
			test++;
		}
		
		afficherRayon(rayon);
		printf("Entrer une marque de produit à supprimer\n");
		scanf("%s", marque);
		viderBuffer();
		while(chercherProduit(rayon,marque)==NULL){
	        printf("La marque que vous avez tappée n'existe pas dans ce rayon, merci de tapper une autre marque\n");
	        printf("Entrer la marque du produit à ajouter\n");
	        scanf("%s", marque);
	        viderBuffer();
	        verifierNom(marque,'M');
	    }
		supprimerProduit(rayon, marque);
	}
}

void supprimerRayonWrapper(T_Magasin *magasin){
	if (magasin->premier==NULL)	printf("Désolé le magasin ne comporte pas de rayon. Veuillez d'abord en créer un...\n");
	else{
		char nom[tailleNomMagasin];
		T_Rayon* rayon;
		int test=0;

		rayon=selectionnerRayon(magasin);

		int a=supprimerRayon(magasin, rayon->nom_rayon,1);
		if (a==1){
			printf("Tous les produits du rayons %s on bien étaient retirés.\n",rayon->nom_rayon);
			printf("Le rayon %s a bien été supprimé du magasin %s",rayon->nom_rayon,magasin->nom);
		}
		else printf("Il y a eu une erreure.\n");
	}	

	
}


void rechercheProduitsWrapper(T_Magasin *magasin){
	float prix_min, prix_max = 3.0;
	printf("Entrer une fouchette de prix\n");
	printf("Prix minimum ?\n");

	prix_min=saisiePrix();
	
	printf("Prix maximum ?\n");
	prix_max=saisiePrix();

	rechercheProduits(magasin, prix_min, prix_max);
}

T_Magasin *testing(){
	T_Magasin *magasin = creerMagasin("Carrefour");
	T_Rayon *r1 = creerRayon("Fromage"), *r2 = creerRayon("Patisserie"), *r3 = creerRayon("Poisson");
	ajouterRayon(magasin, r1);
	ajouterRayon(magasin, r2);
	ajouterRayon(magasin, r3);
	char a='A', b='B';
	T_Produit *p1 = creerProduit("Saint-Nectaire", 3.14, a, 45), *p2 = creerProduit("Comte", 6.28, b, 90), *p3 = creerProduit("Camember", 1.28, a, 24);
	T_Produit *p4 = creerProduit("Gateau", 1.14, a, 45), *p6 = creerProduit("Pain", 0.28, b, 90);
	T_Produit *p7 = creerProduit("Saumon", 7.14, a, 45), *p8 = creerProduit("Fletan", 15.28, b, 90);
	ajouterProduit(r1, p1);
	ajouterProduit(r1, p2);
	ajouterProduit(r1, p3);
	ajouterProduit(r2, p4);
	ajouterProduit(r2, p6);
	ajouterProduit(r3, p7);
	ajouterProduit(r3, p8);
	return magasin;

}

int fusionnerRayonsWrapper(T_Magasin *magasin){
	int nbRayons=0;
	T_Rayon* rayon=magasin->premier;
	while (rayon != NULL){
		nbRayons++;
		rayon=rayon->suivant;
	}

	if (nbRayons<2){
		printf("Désolé, il n'y a pas assez de rayon dans le magasin pour en fusionner...\n");
		return 0;
	}

	//Là on a assez de rayon pour en fusioner

	int test=0;
	T_Rayon* rayon1,*rayon2;
	do{
		if (test!=0) printf("Désolé, vous avez choisi deux fois le même rayon, veuillez recommencer.\n");
		printf("Entrez les noms de rayons à fusionner.\n");
		rayon1=selectionnerRayon(magasin);
		printf("\nEntrez le nom du deuxième rayon.");
		rayon2=selectionnerRayon(magasin);
		test++;
	}while(strcmp(rayon1->nom_rayon,rayon2->nom_rayon)==0); //On recommence temps que l'on a pas sélectionner deux rayons différents

	//A ce niveau on connait les deux rayons à fusionner.

	T_Rayon *rayonFusion=creerRayon("tmp");

	T_Produit *produit1=rayon1->premier, *produit2=rayon2->premier, *actuel; //Actuel stock l'addresse du dernier produit fusionné

	//On commence par éliminer les cas triviaux
	if (produit1 == NULL) rayonFusion->premier=produit2;
	else if (produit2==NULL) rayonFusion->premier=produit1;
	else if (produit1 == NULL && produit2 == NULL) rayonFusion->premier=NULL;
	//Puis on rentre dans le vif du sujet
	else{
		//on raccorde le début du rayon
		if(produit1->prix < produit2->prix){
			rayonFusion->premier=produit1;
			produit1=produit1->suivant;
		}
		else{
			rayonFusion->premier=produit2;
			produit2=produit2->suivant;
		};

		actuel=rayonFusion->premier;
		//on fusionne le reste
		while(produit1 != NULL  && produit2 != NULL){
			if(produit1->prix < produit2->prix){
				actuel->suivant=produit1;
				actuel=actuel->suivant;
				produit1=produit1->suivant;
			}
			else{
				actuel->suivant=produit2;
				actuel=actuel->suivant;
				produit2=produit2->suivant;
			}
		}

		//On complète le rayon si on n'est pas aller au bout de l'un ou de l'autre
		if (produit1 == NULL) actuel->suivant=produit2;
		else actuel->suivant=produit1;
	}
	

	rayonFusion->nombre_produits=rayon1->nombre_produits + rayon2 -> nombre_produits;

	supprimerRayon(magasin,rayon1->nom_rayon,0);//on ne supprime pas (en mémoire) les produits du rayon
	supprimerRayon(magasin,rayon2->nom_rayon,0);//on ne supprime pas (en mémoire) les produits du rayon
	

	//Vérification qu'il n'y a pas deux fois la même marque dans le rayon fusioner
	produit1=rayonFusion->premier;
	produit2=produit1->suivant;
	T_Produit *precedent;

	while (produit1 != NULL){
		produit2=produit1->suivant;
		precedent=produit1;

		while(produit2 != NULL){
			if (strcmp(produit1->marque,produit2->marque)==0){
				//On a deux produits de la même marque

				produit1->quantite_en_stock = produit1->quantite_en_stock + produit2->quantite_en_stock; 
				
				printf("\n \n Les deux produits de la marque %s on été fusionnés en un seul.\n", produit1->marque);
				printf("Les quantités des deux ont été additionnées.\n");
				printf("La qualité du moins cher a été conservé.\n");
				
				//On reforme la chaine
				precedent->suivant=produit2->suivant;
				free(produit2);
				printf("L'espace mémoire du deuxième a été libéré.\n \n \n ");
				produit2=precedent;
				rayonFusion->nombre_produits--;
			}
			else{
				precedent=produit2;
				produit2=produit2->suivant;
			}
			
		}

		produit1=produit1->suivant;
	}



	printf("Choississez un nouveau nom pour votre rayon :\n");
	nouveauNomRayon(magasin, rayonFusion->nom_rayon);
	ajouterRayon(magasin,rayonFusion);

	//Vérifier pas deux fois la même marque
	//A faire
	//penser a faire des free
}



void viderMagasin(T_Magasin *magasin){
	T_Rayon *rayon=magasin->premier, *rPrecedent;
	T_Produit *produit,*pPrecedent;

	while (rayon != NULL){
		
		produit=rayon->premier;
		while (produit != NULL){
			pPrecedent=produit;
			free(produit);
			produit=pPrecedent->suivant;
		}
		
		rPrecedent=rayon;
		free(rayon);
		rayon=rPrecedent->suivant;
	}
	free(magasin);
	printf("Vous avez quittez, tous les espaces mémoires ont été libérés !");
	printf("\n");
}

void main(){
	T_Magasin *magasin;
	int state = 1;
	magasin = testing();
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
            	fusionnerRayonsWrapper(magasin);
            	break;
            case 10:
            	viderMagasin(magasin);
                state = 0;
                break;
            default: 
            	break;
        }
    }
}
