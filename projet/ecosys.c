#include "ecosys.h"

float p_ch_dir = 0.01; 
float p_reproduce_proie = 0.4; //definir les probas
float p_reproduce_predateur = 0.5;
int temps_repousse_herbe = -15;

Animal *creer_animal(int x, int y, float energie) {
  Animal *a1 = (Animal *) malloc(sizeof(Animal));
  assert(a1);
  a1->x = x;
  a1->y = y;
  a1->energie = energie;
  a1->dir[0] = rand() % 3 - 1; //rand entre -1 et 1
  a1->dir[1] = rand() % 3 - 1; //rand entre -1 et 1
  a1->suivant = NULL;
  return a1;
}


Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  assert(animal);
  animal->suivant = liste;
  return animal;
}

unsigned int compte_animal_rec(Animal *la) {  
  if (!la) return 0; 
  return 1 + compte_animal_rec(la->suivant);
}


unsigned int compte_animal_it(Animal *la) {
  int c=0;
  while (la) {
    c++;
    la=la->suivant;
  }
  return c;
}


void ajouter_animal(int x, int y, Animal** liste_animal){
  assert((x < SIZE_X) && (y < SIZE_Y)); //test x et y dans les bornes
  float energie = ((float)rand() * (float)(RAND_MAX)) * 10; //energie rand
  *liste_animal = ajouter_en_tete_animal(*liste_animal, creer_animal(x, y, energie));
}


Animal* liberer_liste_animaux(Animal *liste) { 
  Animal* iter = liste;

  while(liste){ //libere toute la liste en iterant
      iter = liste->suivant;
      free(liste);
      liste = iter;
  }

  return liste;
}


void enlever_animal(Animal **liste, Animal *animal) { /////////////////////////////////
  Animal* temp = (Animal*)malloc(sizeof(Animal));

    if (*liste == animal){ //pour effacer si animal dans 1er
       temp = (*liste)->suivant;
       free(*liste);
       *liste = temp;
    }

    else{ //si animal pas au debut
        Animal* tete = (Animal*)malloc(sizeof(Animal));
        tete = *liste; 
        while (*liste){
            if (*liste == animal){ //si on trouve animal
                temp->suivant = (*liste)->suivant;
                free(*liste);
                *liste = temp->suivant;
            }

            else{ //si non on continue
                temp = *liste;
                *liste = (*liste)->suivant;
            }
        }

        *liste = tete; //retour en tete
    }
}

void init_ecosysteme(char eco[SIZE_X][SIZE_Y]){
    for (int i = 0; i < SIZE_X; i++){ //espace pour tout les cases
      for (int j = 0; j < SIZE_Y; j++){
        eco[i][j] = ' ';
      }
    }
}


void remplir_ecosysteme(char eco[SIZE_X][SIZE_Y], Animal* liste_proies, Animal* liste_predateurs){  ///////////////////////////
    int new_x, new_y;

    init_ecosysteme(eco);

    while(liste_proies){
        new_x = liste_proies->x; //coordonnees des proies
        new_y = liste_proies->y;

        assert(((0 <= new_x) && (new_x < SIZE_X)) && ((0 <= new_y) && (new_y < SIZE_Y))); //test tout est dans les bornes

        eco[new_x][new_y] = '*'; //ecriture des proies dans les coordonnes 

        liste_proies = liste_proies->suivant; //continue boucle
    }

    while (liste_predateurs){
       
        new_x = liste_predateurs->x; //coordonnees des pred
        new_y = liste_predateurs->y;

        assert((new_x < SIZE_X) && (new_y < SIZE_Y)); //test tout est dans les bornes

        if ((eco[new_x][new_y] == '*') || (eco[new_x][new_y] == '@')){ //il y a deja une proie dans la case
            eco[new_x][new_y] = '@'; //proie et pred
        }

        else{
            eco[new_x][new_y] = 'O'; //seulement pred
        }

        liste_predateurs = liste_predateurs->suivant;
    }
}


void ecrire_ecosys(const char *nom_fichier, Animal *liste_predateur, Animal *liste_proie) {
    FILE *f=fopen(nom_fichier, "w"); 
   
    fprintf(f,"<proies>\n"); //ecriture 1er ligne <proies>
    Animal *ani=liste_proie; 

    while (ani!=NULL) { //ecriture de chaque proie dans f
        fprintf(f,"x=%d y=%d dir=[%d %d] e=%f\n",ani->x, ani->y, ani->dir[0], ani->dir[1], ani->energie);
        ani=ani->suivant;
    }

    fprintf(f,"</proies>\n"); 
    fprintf(f,"<predateurs>\n");
    ani=liste_predateur; //pour lire liste pred

    while (ani) { //ecriture de chaque pred
        fprintf(f,"x=%d y=%d dir=[%d %d] e=%f\n",ani->x, ani->y, ani->dir[0], ani->dir[1], ani->energie);
        ani=ani->suivant;
    }

    fprintf(f,"</predateurs>\n"); //ecriture derniere ligne
    fclose(f);
}



void lire_ecosys(const char* nom_fichier, Animal** liste_predateur, Animal** liste_proie){  ////////////////////////////////////////////////

    assert((compte_animal_rec(*liste_proie) == 0) && (compte_animal_rec(*liste_predateur) == 0)); //test listes deja vides

    FILE* txt = fopen(nom_fichier, "r");

    if (txt == NULL){
        printf("Le fichier est nulle %s\n", nom_fichier);
    }

    else{
        char buffer[128];
        fgets(buffer, 128, txt); //lire 1er ligne <proies>

        int x, y, dir0, dir1; //pour les donnees d'animaux qu'on va lire
        float nrg;

        char* res = fgets(buffer, 128, txt);
        while (strlen(res) > 10){ //lire jusqu'a </proies>
            sscanf(buffer, "x=%d y=%d dir=[%d %d] e=%f", &x, &y, &dir0, &dir1, &nrg); //lire donnees dex animaux

            
            Animal* ani = (Animal*)malloc(sizeof(Animal));
            ani->x = x;
            ani->y = y;
            ani->dir[0] = dir0; //creation d'animal avec les donnees lus
            ani->dir[1] = dir1;
            ani->energie = nrg;
            ani->suivant = NULL;

            *liste_proie = ajouter_en_tete_animal(*liste_proie, ani); //ajout de nouveau animal

            
            res = fgets(buffer, 128, txt); //continue a lire avec la ligne suivante
        }

        res = fgets(buffer, 128, txt); 
        res = fgets(buffer, 128, txt); //lit les deux lignes  </proies> et <predateurs>

        while(strlen(res) > 14){ //lire jusqu'a </predateurs>
            sscanf(buffer, "x=%d y=%d dir=[%d %d] e=%f", &x, &y, &dir0, &dir1, &nrg);  //lire donnees dex animaux

            
            Animal* ani = (Animal*)malloc(sizeof(Animal));
            ani->x = x;
            ani->y = y;
            ani->dir[0] = dir0;
            ani->dir[1] = dir1;//creation d'animal avec les donnees lus
            ani->energie = nrg;
            ani->suivant = NULL;

            *liste_predateur = ajouter_en_tete_animal(*liste_predateur, ani); //ajout de nouveau animal

           
            res = fgets(buffer, 128, txt); //continue boucle
        }

        
        res = fgets(buffer, 128, txt); //lit derniere ligne </predateurs>
        assert(res == NULL); //tout est lu
    }

    fclose(txt);
}

void afficher_ecosysteme(char tab[SIZE_X][SIZE_Y], Animal* liste_proies, Animal* liste_predateurs){ /////////////////////////////////////////////

    

    int nbPro = compte_animal_rec(liste_proies);
    int nbPred = compte_animal_rec(liste_predateurs); //pour garder les nb

    for (int i = -1; i <= SIZE_X; i++){
        for (int j = -1; j <= SIZE_Y; j++){
            if ((i == -1) || (i == SIZE_X)){
                if ((j == -1) || (j == SIZE_Y)){
                    printf("+ "); //affiche les coins
                }

                else{
                    printf("- "); //affiche les cotes 
                }
            }

            else if ((j == -1) || (j == SIZE_Y)){
                printf("| "); //affiche les cotes
            }

            else{
                printf("%c ", tab[i][j]); //affiche contenu tab
            }
        }

        printf("\n");
    }

    printf("Proies(*): %d \t Predateurs(O): %d\n", nbPro, nbPred);
}


void clear_screen() {
  printf("\x1b[2J\x1b[1;1H");  // code ANSI X3.4 pour effacer l'ecran 
}



void bouger_animaux(Animal *la) {
  Animal *ap = la;
  while (ap) {
      if (rand() / (float)RAND_MAX < p_ch_dir) {  // changement avec une proba p_ch_dir
          ap->dir[0] = rand() % 3 - 1; //changement horizontale si proba
          ap->dir[1] = rand() % 3 - 1; //changement verticale si proba
      }

      ap->x = (ap->x + ap->dir[0] + SIZE_X) % SIZE_X; //applique changement
      ap->y = (ap->y + ap->dir[1] + SIZE_Y) % SIZE_Y; //applique changement
      ap = ap->suivant;
  }

}


void reproduce(Animal **liste_animal, float p_reproduce) {
  Animal *ap = (Animal*)malloc(sizeof(Animal));
  ap = *liste_animal;
  while (*liste_animal){
        if ((float)rand() / (float)(RAND_MAX) < p_reproduce){ //avec proba p_reproduce
          Animal* ani = (Animal*)malloc(sizeof(Animal));

          ani->x = (*liste_animal)->x;
          ani->y = (*liste_animal)->y;
          ani->dir[0] = (*liste_animal)->dir[0]; //tout sauf lenergie est identique a sa mere
          ani->dir[1] = (*liste_animal)->dir[1];
          ani->energie = (*liste_animal)->energie / 2;

           ap = ajouter_en_tete_animal(ap, ani); //ajout du bebe

        
          (*liste_animal)->energie /= 2; // diminue energie mere
          *liste_animal = (*liste_animal)->suivant;
        }

        else{ //si pas proba
          *liste_animal = (*liste_animal)->suivant;
        }
    }
    *liste_animal = ap; //retour en tete
}


void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) { /////////////////////////////////////////////////////////////////////////////
    // if the energy level of a proie is lower than 0, then it shall be dead (deleted from the list "*liste_proie")
    // if there is grass (grass data taken form "monde") in the same position as proie, than proie eats the grass and increases its energy by the grass amount
    // finally call the reproduction function "reproduce()" to let some proies reproduce

    bouger_animaux(*liste_proie); //bouge tout le monde

    Animal* tete = (Animal*)malloc(sizeof(Animal));
    tete = *liste_proie; 
    while (*liste_proie){
        (*liste_proie)->energie -= 1; //diminue lenergie

        if (monde[(*liste_proie)->x][(*liste_proie)->y] >= 0){ //sil y a d'herbes dans les coord d'animal l'energie augmente
            (*liste_proie)->energie += monde[(*liste_proie)->x][(*liste_proie)->y]; 
            // assert(temps_repousse_herbe < 0); //grass reset variable test
            // monde[(*liste_proie)->x][(*liste_proie)->y] = temps_repousse_herbe; //reseting grass data (case set to "temps_repousse_herbe")
        }

        if ((*liste_proie)->energie <= 0){
            enlever_animal(&tete, *liste_proie); //si lenergie est inf a 0 il meurt :(
        

        *liste_proie = (*liste_proie)->suivant; //continue boucle
    }

    *liste_proie = tete; //retour en tete

    reproduce(liste_proie, p_reproduce_proie); //reproduction
}

}


Animal *animal_en_XY(Animal *l, int x, int y) {

  Animal *iter = l;

  while(iter){
    if (iter->x == x && iter->y == y){ //trouve lanimal
      return iter; 
    }
    iter = iter->suivant;
  }

  return NULL; //si animal pas trouve return NULL
} 


void rafraichir_predateurs(Animal** liste_predateur, Animal** liste_proie){///////////////////////////////////////////////////////
    // move all the predators calling "bouger_animaux()" present in "*liste_predateur" while decreasing each one's energy by 1
    // if the energy level of a predator is lower than 0, then it shall be dead (deleted from the list "*liste_predateur")
    // if there is a proie in the same position with the predator than the predator eats the proie (death of the proie and release of memory occupied by the proie) and gets its energy
    // finally call the reproduction function "reproduce()" to let some predators reproduce
    // (similar principles with the function "rafraichir_proies()")
    

    bouger_animaux(*liste_predateur); //bouge tout le monde

    Animal* tete = (Animal*)malloc(sizeof(Animal));
    tete = *liste_predateur; 
    while (*liste_predateur){
        (*liste_predateur)->energie -= 1; //diminue lenergie

        if ((*liste_predateur)->energie <= 0){
            enlever_animal(&tete, *liste_predateur); //si lenergie est inf a 0 il meurt :(
        }

        *liste_predateur = (*liste_predateur)->suivant;
    }

    *liste_predateur = tete; //retour tete

    while (*liste_predateur){
        Animal* proie = animal_en_XY(*liste_proie, (*liste_predateur)->x, (*liste_predateur)->y); //check sil y a proie et predateur en m temps

        if (proie){ //sil y a proie
            float nrg = proie->energie; 
            (*liste_predateur)->energie += nrg; //le predateur augmente sa energie avec celle du proie
            enlever_animal(liste_proie, proie); //mort du proie
        }

        *liste_predateur = (*liste_predateur)->suivant;
    }

    *liste_predateur = tete; 

    reproduce(liste_predateur, p_reproduce_predateur);
}


void rafraichir_monde(int monde[SIZE_X][SIZE_Y]){
    // augmente tout les cases par 1 

    for (int i = 0; i < SIZE_X; i++){
        for (int j = 0; j < SIZE_Y; j++){
            monde[i][j]++;
        }
    }
}
