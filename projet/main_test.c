#include "ecosys.h"


int main(){
    // external variables taken from "ecosys.h"
    float p_reproduce_proie = 0.4;
    float p_reproduce_predateur = 0.5;

    char ecosys[SIZE_X][SIZE_Y]; //init l'ecosysteme
    init_ecosysteme(ecosys);

    
    printf("Ecosysteme vide:\n");
    afficher_ecosysteme(ecosys, NULL, NULL); //affiche eco vide
    printf("\n");

    
    srand(time(NULL));
    int x = rand() % (SIZE_X); //coordonnes random 0 sizex-1
    int y = rand() % (SIZE_Y); //coordonnes random
    float energie = ((float)rand() / (float)(RAND_MAX)) * 10; 

    Animal* liste_proies = malloc(sizeof(Animal)); //creation de lanimal
    liste_proies = creer_animal(x, y, energie); //ajout dans la liste
    
    for (int i = 0; i < 14; i++){ //creation de 14 autre proies
        x = rand() % (SIZE_X);
        y = rand() % (SIZE_Y);
        energie = ((float)rand() / (float)(RAND_MAX)) * 10;

        liste_proies = ajouter_en_tete_animal(liste_proies, creer_animal(x, y, energie));        
    }

   
    x = rand() % (SIZE_X); 
    y = rand() % (SIZE_Y); //le meme pour des predateurs
    energie = ((float)rand() / (float)(RAND_MAX)) * 10; 

    Animal* liste_predateurs = malloc(sizeof(Animal));
    liste_predateurs = creer_animal(x, y, energie);
    
    for (int i = 0; i < 14; i++){
        x = rand() % (SIZE_X);
        y = rand() % (SIZE_Y);
        energie = ((float)rand() / (float)(RAND_MAX)) * 10;

        liste_predateurs = ajouter_en_tete_animal(liste_predateurs, creer_animal(x, y, energie));        
    }

    
    printf("Nb proies au debut: %d\n", compte_animal_rec(liste_proies)); 
    printf("Nb predateurs au debut: %d\n", compte_animal_rec(liste_predateurs)); //compte des proies et predateurs
    printf("\n");

    
    remplir_ecosysteme(ecosys, liste_proies, liste_predateurs); //rempli eco avec proies et predateurs

    
    printf("Ecosysteme avec 15 proies et 15 predateurs:\n");
    afficher_ecosysteme(ecosys, liste_proies, liste_predateurs); //affiche eco apres l'avoir rempli  
    printf("\n");


    Animal* pro1 = creer_animal(rand() % (SIZE_X), rand() % (SIZE_Y), ((float)rand() / (float)(RAND_MAX)) * 10);
    liste_proies = ajouter_en_tete_animal(liste_proies, pro1); 
    //creation d'un proie et un predateur pour ajouter apres l'affichage
    Animal* pred1 = creer_animal(rand() % (SIZE_X), rand() % (SIZE_Y), ((float)rand() / (float)(RAND_MAX)) * 10);
    liste_predateurs = ajouter_en_tete_animal(liste_predateurs, pred1); 

    printf("Nb proies apres la creation de pro1: %d\n", compte_animal_rec(liste_proies)); //test ajout
    printf("Nb predateurs apres la creation de pred1: %d\n", compte_animal_it(liste_predateurs)); //test ajout 
    printf("\n");

    remplir_ecosysteme(ecosys, liste_proies, liste_predateurs); //ajout de nouveau predateur et proie
    printf("Ecosysteme apres l'ajout d'un predateur et une proie:\n"); 
    afficher_ecosysteme(ecosys, liste_proies, liste_predateurs);
    printf("\n");

    enlever_animal(&liste_proies, pro1);
    enlever_animal(&liste_predateurs, pred1); //efface les derniers animaux ajoutes

    printf("Nb proies apres l'enlevement de pro1: %d\n", compte_animal_rec(liste_proies));
    printf("Nb predateurs apres l'enlevement de pred1: %d\n", compte_animal_rec(liste_predateurs));
    printf("\n");

    
    ecrire_ecosys("fileEcosys.txt", liste_predateurs, liste_proies); //ecriture dans file

    remplir_ecosysteme(ecosys, liste_proies, liste_predateurs);
    printf("Ecosysteme juste avant la liberation des listes:\n");
    afficher_ecosysteme(ecosys, liste_proies, liste_predateurs);
    printf("\n");

    liste_proies = liberer_liste_animaux(liste_proies); //efface liste
    liste_predateurs = liberer_liste_animaux(liste_predateurs); //efface liste 

    printf("Nb proies apres la liberation: %d\n", compte_animal_rec(liste_proies));
    printf("Nb predateurs apres la liberation: %d\n", compte_animal_rec(liste_predateurs)); //0 pred 0 proie
    printf("\n");

    remplir_ecosysteme(ecosys, NULL, NULL);
    printf("Ecosysteme apres la suppression de tous les animaux:\n"); //affichage vide
    afficher_ecosysteme(ecosys, liste_proies, liste_predateurs);
    printf("\n");

    lire_ecosys("fileEcosys.txt", &liste_predateurs, &liste_proies); //lire le file ou on avait ecrit 

    printf("Le nombre de proies apres la lecture du fichier: %d\n", compte_animal_rec(liste_proies)); //compte d'apres le file
    printf("Le nombre de predateurs apres la lecture du fichier: %d\n", compte_animal_rec(liste_predateurs));
    printf("\n");


    remplir_ecosysteme(ecosys, liste_proies, liste_predateurs); //rempli d'apres la lecture du file
    printf("Ecosysteme apres la lecture du fichier:\n"); 
    afficher_ecosysteme(ecosys, liste_proies, liste_predateurs); //affiche d'apres la lecture du file
    printf("\n");

    bouger_animaux(liste_proies);
    remplir_ecosysteme(ecosys, liste_proies, liste_predateurs);
    printf("Ecosysteme apres le mouvement des proies:\n"); 
    afficher_ecosysteme(ecosys, liste_proies, liste_predateurs); //affiche apres bouger_animaux
    printf("\n");

    bouger_animaux(liste_predateurs);
    remplir_ecosysteme(ecosys, liste_proies, liste_predateurs);
    printf("Ecosysteme apres le mouvement des predateurs:\n");
    afficher_ecosysteme(ecosys, liste_proies, liste_predateurs);
    printf("\n");

    
    reproduce(&liste_proies, p_reproduce_proie);
    reproduce(&liste_predateurs, p_reproduce_predateur);
    remplir_ecosysteme(ecosys, liste_proies, liste_predateurs);
    printf("Ecosysteme apres la reproduction des animaux:\n");
    afficher_ecosysteme(ecosys, liste_proies, liste_predateurs); //affiche apres reproduction
    printf("\n");

    return 0;
}