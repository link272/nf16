# include "tp4.c"
# include "tp4.h"
# include "tp4_cli.c"

int afficherMenu(){

    int choix = 0;

    printf("\n---Menu---\n\n");

    printf("1.Initialiser un arbre\n");
    printf("2.Ajouter une Tranche\n");
    printf("3.Ajouter un bénévole dans une tranche d’âge\n");
    printf("4.Afficher les tranches d’âge d’un ABR\n");
    printf("5.Afficher les bénévoles d’une tranche d’âge\n");
    printf("6.Supprimer un bénévole\n");
    printf("7.Supprimer une tranche\n");
    printf("8.Afficher les bénévoles d’honneur\n");
    printf("9.Afficher le nombre total de bénévoles\n");
    printf("10.Actualiser l’ABR\n");
    printf("11.Afficher le pourcentage de bénévoles d’une tranche d’âge\n");
    printf("12.Détruire l’arbre\n");
    printf("13.Quitter\n\n");
    printf("Exprimer votre choix: ");
    scanf("%d", &choix);
    viderBuffer();
    
    return choix;
}

void main(){
	T_Tranche *arbre;
	int state = 1;
	arbre = testing();
	printf("##########################################################\n");
   	printf("Un arbre suivant le protocole d'évaluation a été chargé.\n");
    	printf("##########################################################\n");

    while(state){
        switch(afficherMenu()){
            case 1:
            	viderArbre(arbre);
                arbre = initArbreCLI();
                break;
            case 2:
            	ajouterTrancheCLI(arbre);
                break;
            case 3:
                ajouterBenCLI(arbre);
                break;
            case 4:
                afficherTrancheCLI(arbre);
                break;
            case 5:
                afficherBenCLI(arbre);
                break;
            case 6:
                supprimerBenCLI(arbre);
                break;
            case 7:
                supprimerTrancheCLI(arbre);
                break;
            case 8:
                AfficherBestBenCLI(arbre);
                break;
            case 9:
            	ActualiserArbre(arbre);
            	break;
            case 10:
                AfficherPercentBen(arbre);
                break;
            case 11:
                AfficherBestBenCLI(arbre);
                break;
            case 12:
                ViderArbre(arbre);
                break;
            case 13:
            	viderArbre(arbre);
                state = 0;
                break;
            default: 
            	break;
        }
    }
}
