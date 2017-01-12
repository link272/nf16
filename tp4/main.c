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
