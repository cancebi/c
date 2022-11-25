#include "ecosys.h"


int main(){

    char ecosys[SIZE_X][SIZE_Y]; 
    init_ecosysteme(ecosys); //initialisation

    printf("Ecosysteme vide:\n");
    afficher_ecosysteme(ecosys, NULL, NULL); //affiche eco vide
    printf("\n");

    srand(time(NULL));
    float energie = ((float)rand() / (float)(RAND_MAX)) * 10; 

    Animal* proie = malloc(sizeof(Animal));
    proie = creer_animal(5, 5, energie); //cree proie
    
    
    proie->dir[0] = 1; //coordonnes proie
    proie->dir[1] = 1;

    remplir_ecosysteme(ecosys, proie, NULL); //remplir avec le seul proie

    printf("Ecosysteme avec 1 proie:\n");
    afficher_ecosysteme(ecosys, proie, NULL); //affiche avec le seul proie
    printf("\n");


    bouger_animaux(proie); //bouge le seul proie
    remplir_ecosysteme(ecosys, proie, NULL);
    printf("Ecosysteme apres le mouvement de la proie:\n");
    afficher_ecosysteme(ecosys, proie, NULL);
    printf("\n");

    
    proie->x = 9;
    proie->y = 9;
    proie->dir[0] = -1; //bouge manuellement
    proie->dir[1] = -1; 

    remplir_ecosysteme(ecosys, proie, NULL);
    printf("Ecosysteme apres le repositionnement de la proie:\n"); 
    afficher_ecosysteme(ecosys, proie, NULL); //affiche apres bouge manuel
    printf("\n");

    
    bouger_animaux(proie); 
    remplir_ecosysteme(ecosys, proie, NULL);
    printf("Ecosysteme apres le deuxieme deplacement de la proie:\n");
    afficher_ecosysteme(ecosys, proie, NULL);
    printf("\n");

    reproduce(&proie, 1); //reproduction garantit avec 1
    remplir_ecosysteme(ecosys, proie, NULL);
    printf("Ecosysteme apres la reproduction de la proie:\n");
    afficher_ecosysteme(ecosys, proie, NULL); //affiche ne change pas car bebe est dans les memes coordonnes mais Nb change
    printf("\n");

    proie = liberer_liste_animaux(proie); //efface liste
    remplir_ecosysteme(ecosys, proie, NULL);
    printf("Ecosysteme apres la suppression des proies:\n"); 
    afficher_ecosysteme(ecosys, proie, NULL); //affiche null
    printf("\n");

   
    int x = rand() % (SIZE_X ); 
    int y = rand() % (SIZE_Y); 
    energie = ((float)rand() / (float)(RAND_MAX)) * 10; 

    Animal* liste_proies = malloc(sizeof(Animal));
    liste_proies = creer_animal(x, y, energie);
    
    for (int i = 0; i < 14; i++){
        x = rand() % (SIZE_X);
        y = rand() % (SIZE_Y);
        energie = ((float)rand() / (float)(RAND_MAX)) * 10;

        liste_proies = ajouter_en_tete_animal(liste_proies, creer_animal(x, y, energie));        
    }

    
    remplir_ecosysteme(ecosys, liste_proies, NULL);
    printf("Ecosysteme apres la creation de 20 proies:\n");
    afficher_ecosysteme(ecosys, liste_proies, NULL); //affiche avec seulement proies
    printf("\n");

    
    int herbe[SIZE_X][SIZE_Y];

    for (int i = 0; i < SIZE_X; i++){
        for (int j = 0; j < SIZE_Y; j++){ //creation d'herbes avec 0 
            herbe[i][j] = 0;
        }
    }

  
    printf("\n\n\n---DEBUT DE LA BOUCLE---\n\n");

    int i = 0;
    while ((i < 200) && (compte_animal_rec(liste_proies) != 0)){
        rafraichir_monde(herbe); //herbes poussent

        rafraichir_proies(&liste_proies, herbe);

        remplir_ecosysteme(ecosys, liste_proies, NULL);

        printf("Ecosysteme en %d. etape de boucle:\n", (i + 1));
        afficher_ecosysteme(ecosys, liste_proies, NULL);
        printf("\n");

        i++;
    }

    printf("---FIN DE LA BOUCLE (%d etapes)---\n\n\n\n", i);

    printf("Ecosysteme apres la boucle:\n");
    afficher_ecosysteme(ecosys, liste_proies, NULL);
    printf("\n");

    liste_proies = liberer_liste_animaux(liste_proies); //efface liste

    for (int i = 0; i < 20; i++){ //creation 20 proies
        x = rand() % (SIZE_X);
        y = rand() % (SIZE_Y);
        energie = ((float)rand() / (float)(RAND_MAX)) * 10;

        liste_proies = ajouter_en_tete_animal(liste_proies, creer_animal(x, y, energie));        
    }
    
    Animal* liste_predateurs = malloc(sizeof(Animal)); 
    liste_predateurs = NULL;
    
    for (int i = 0; i < 20; i++){ //creation 20 predateurs
        x = rand() % (SIZE_X);
        y = rand() % (SIZE_Y);
        energie = ((float)rand() / (float)(RAND_MAX)) * 10;

        liste_predateurs = ajouter_en_tete_animal(liste_predateurs, creer_animal(x, y, energie));        
    }

    remplir_ecosysteme(ecosys, liste_proies, liste_predateurs);
    printf("Ecosysteme apres la creation de 20 proies and 20 predateurs:\n");
    afficher_ecosysteme(ecosys, liste_proies, liste_predateurs);
    printf("\n");

    for (int i = 0; i < SIZE_X; i++){
        for (int j = 0; j < SIZE_Y; j++){
            herbe[i][j] = 0; //stop herbes
        }
    }

    // loop for the functions "rafraichir_proies()", "rafraichir_monde()" and "rafraichir_predateurs()" (stop after 200 iterations OR when there is no more preys or predators left)
    printf("\n\n\n---DEBUT DE LA BOUCLE---\n\n");

    FILE* evol = fopen("evolPop.txt", "w"); //opening the file "evolPop.txt" as evol to store loop data for the graph
    assert(evol != NULL); 

    i = 0; 
    while ((i < 200) && ((compte_animal_rec(liste_proies) != 0) && (compte_animal_rec(liste_predateurs) != 0))){
        
        rafraichir_monde(herbe);
        rafraichir_proies(&liste_proies, herbe); //refresh tout
        rafraichir_predateurs(&liste_predateurs, &liste_proies);

        // writing loop data to the file "evolPop.txt"
        fprintf(evol, "%d %d %d\n", i, compte_animal_rec(liste_proies), compte_animal_rec(liste_predateurs));

        remplir_ecosysteme(ecosys, liste_proies, liste_predateurs);

        // displaying "ecosys" on each iteration
        printf("Ecosysteme en %d. etape de boucle:\n", (i + 1));
        afficher_ecosysteme(ecosys, liste_proies, liste_predateurs);
        printf("\n");

        i++;
    }

    fclose(evol); 

    printf("---FIN DE LA BOUCLE (%d etapes)---\n\n\n\n", i);

    
    liste_proies = liberer_liste_animaux(liste_proies); //efface listes
    liste_predateurs = liberer_liste_animaux(liste_predateurs);

    assert((compte_animal_rec(liste_proies) == 0) && (compte_animal_rec(liste_predateurs) == 0)); //test efface

    
    remplir_ecosysteme(ecosys, liste_proies, liste_predateurs);
    printf("Ecosysteme vide a la fin:\n");
    afficher_ecosysteme(ecosys, liste_proies, liste_predateurs); //affiche vid
    printf("\n");

    return 0;
}