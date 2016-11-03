#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define taillePARK 30
#define tailleNomMarque 30
#define tailleNomRayon 100
#define tailleNomMagasin 100

typedef struct Produit T_Produit;
struct Produit{
	char marque[tailleNomMarque];
	float prix;
	char qualite; //valeur possible A, B ou C
	int quantite_en_stock;
	T_Produit* suivant; //Pointeur vers le produit suivant dans le rayon
};

typedef struct Rayon T_Rayon;
struct Rayon{
	char nom_rayon[tailleNomRayon];
	int nombre_produits; 
	//nombre de marques de produit distinctes dans le même rayon;
	//on considère que l'on ne peut pas avoir deux produits de la même marque dans un même rayon
	T_Produit* premier; //Pointeur vers le permier produit
	T_Rayon* suivant; //Pointeur vers le rayon suivant
};

typedef struct Magasin T_Magasin;
struct Magasin{
	char nom[tailleNomMagasin];
	T_Rayon* premier; //Pointeur vers le premier rayon du magasin
};

typedef struct Requete T_Requete;
struct Requete{
	T_Requete *suivant;
	T_Requete *precedent;
	T_Produit *produit;
	T_Rayon *rayon;
};



//Création et initialisation des structures
T_Produit *creerProduit(char *marque, float prix, char qualite, int quantite);
T_Rayon *creerRayon(char *nom);
T_Magasin *creerMagasin(char *nom);


//Ajout d'un rayon dans un magasin
int ajouterRayon(T_Magasin *magasin, T_Rayon *rayon);
	//renvoie 1 si l'ajout s'est bien passé, 0 sinon; l'ajout se fait en respectant le tri par ordre alphabétique sur le nom du
	//rayon; on ne doit pas autoriser l'utilisateur à ajouter deux fois le même rayon


//Ajout d'un produit dans un rayon
int ajouterProduit(T_Rayon *rayon, T_Produit *produit);
	//renvoie 1 si l'ajout s'est bien passé, 0 sinon; l'ajout se fait en respectant le tri par ordre croissant du prix du produit;
	//on ne doit pas autoriser l'utilisateur à ajouter deux fois la même marque de produit dans un rayon


//Affichage de tous les rayons d'un magasin
void afficherMagasin(T_Magasin *magasin);
	//L'affichage se fait sous forme de liste triée sur le nom des rayons


//Affichage de tous les produits d'un rayon
void afficherRayon(T_Rayon *rayon);
	//L'affichage se fait sous forme de liste triée sur le prix du produit


//Suppresion d'un produit dans un rayon
int supprimerProduit(T_Rayon *rayon, char* marque_produit);
	//renvoie 1 si le retrait s'est bien passé, 0 sinon; on veillera à libérer la mémoire précédemment allouée


//Suppression d'un rayon et de tous les produits qu'il contient
int supprimerRayon(T_Magasin *magasin, char *nom_rayon);
	//renvoie 1 si la suppression s'est bien passée, 0 sinon; on veillera à libérer la mémoire précédemment allouée


//Recherche de produits se situant dans une fourchette de prix entrée par l'utilisateur
void rechercheProduits(T_Magasin *magasin, float prix_min, float prix_max);
	//L'affichage se fait sous forme de liste triée par ordre croissant de prix du produit.
	//Vous expliquerez dans votre rapport votre choix de créer des structures spécifiques ou d'utiliser les structures déjà définies
	//pour cette fonction. Vous veillerez à optimiser cette fonction de manière à effectuer le minimum d'opérations possible.


//Fusionner deux rayons
void fusionnerRayons(T_Magasin *magasin);
	//L'utilisateur choisit deux rayons parmi ceux du magasin. On fusionne alors ces deux rayons en un seul dont le nom
	//sera au choix de l'utilisateur et qui contiendra les produits des deux rayons fusionnés triés par ordre croissant de prix.
	//Vous veillerez à optimiser cette fonction de manière à effectuer le minimum d'opérations possible.


//Pour vérifier que la longueur des noms rentrés est la bonne
char* verifierNom(char* nom,char type);


T_Requete *creerRequete(T_Produit *produit, T_Rayon *rayon);
void afficherRequete(T_Requete *sentinelle);
void trierRequete(T_Requete *sentinelle);
void viderBuffer();