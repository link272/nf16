//PARTIE INTERACTIVE-----------------------------------------------------------------------------------------------------------------------


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
    printf("10.Quitter\n\n");
    printf("Exprimer votre choix: ");
    scanf("%d", &choix);
    viderBuffer();
    
    return choix;
}



////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
