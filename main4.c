
void main(){
	T_Magasin *magasin;
	int state = 1;
	magasin = testing();
	printf("##########################################################\n");
   	printf("Un magasin suivant le protocole d'évaluation a été chargé.\n");
    	printf("##########################################################\n");

    while(state){
        switch(afficherMenu()){
            case 1:
            	viderMagasin(magasin);
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
