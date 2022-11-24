#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "ecosys.h"
#define SIZE_X = 20
#define SIZE_Y = 50

Animal *creer_animal(int x, int y, float energie) {
  Animal *a1 = (Animal *) malloc(sizeof(Animal));
  assert(a1);
  a1->x = x;
  a1->y = y;
  a1->energie = energie;
  a1->dir[0] = rand() % 3 - 1;
  a1->dir[1] = rand() % 3 - 1;
  a1->suivant = NULL;
  return a1;
}


Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  assert(animal);
  assert(!animal->suivant);
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


void ajouter_animal(int x, int y, float energie, Animal** liste_animal){
    assert((x < SIZE_X) && (y < SIZE_Y));
    float energie = ((float)rand() * (float)(RAND_MAX)) * 10; 
    *liste_animal = ajouter_en_tete_animal(*liste_animal, creer_animal(x, y, energie));
}


Animal* liberer_liste_animaux(Animal *liste) {
  Animal *efface=liste;
  Animal *iter=liste->suivant;
  while(efface!=NULL){
    free(efface);
    efface=iter;
    iter=iter->suivant;
  }

}


void enlever_animal(Animal **liste, Animal *animal) {
  Animal* iter=(Animal *)malloc(sizeof(Animal));
  Animal* efface=(Animal *)malloc(sizeof(Animal));

  if(*liste==animal){
    iter=(*liste)->suivant;
    free(*liste);
  }
  else{
    iter=*liste;
    while (iter->suivant!=NULL)
    {
      if(iter->suivant == animal){
        efface=iter->suivant;
        iter=efface->suivant;
        free(efface);
      }
      iter=iter->suivant;
    }
    
  }
}




void ecrire_ecosys(const char *nom_fichier, Animal *liste_predateur, Animal *liste_proie) {
    FILE *f=fopen(nom_fichier, "w"); 
   
    fprintf(f,"<proies>\n");
    Animal *a=liste_proie; 

    while (a!=NULL) {
        fprintf(f,"x=%d y=%d dir=[%d %d] e=%f\n",a->x, a->y, a->dir[0], a->dir[1], a->energie);
        a=a->suivant;
    }

    fprintf(f,"</proies>\n"); 
    fprintf(f,"<predateurs>\n");
    a=liste_predateur;

    while (a) { 
        fprintf(f,"x=%d y=%d dir=[%d %d] e=%f\n",a->x, a->y, a->dir[0], a->dir[1], a->energie);
        a=a->suivant;
    }

    fprintf(f,"</predateurs>\n");
    fclose(f);
}



void lire_ecosys(const char *nom_fichier, Animal **liste_predateur, Animal **liste_proie) {

    FILE *f=fopen(nom_fichier, "r");

    if (f==NULL) {
        printf("Erreur lors de l’ouverture de %s\n",nom_fichier);
        return;
    }

    char slist[256];
    int new_x, new_y, new_dir[2];
    float e_lu;

    fgets(slist, 256, f); 
    assert(strncmp(slist, "<proies>",8)==0);
    fgets(slist, 256, f); 
    
    while (strncmp(slist,"</proies>",9)!=0) {
        sscanf(slist,"x=%d y=%d dir=[%d %d] e=%f\n", &new_x, &new_y, &new_dir[0], &new_dir[1], &e_lu);
        Animal *new_a=creer_animal(new_x, new_y, e_lu);
        new_a->dir[0]=new_dir[0];
        new_a->dir[1]=new_dir[1];
        //inserting at the top of the list
        new_a->suivant=*liste_proie;
        *liste_proie = new_a;
        fgets(slist, 256, f); //reading the line containing the next prey
    }

    fgets(slist, 256, f); //reading of <predateurs>
    assert(strncmp(slist, "<predateurs>",12)==0);

    fgets(slist, 256, f); //reading the line containing the first predator

    while (strncmp(slist,"</predateurs>",13)!=0) {
        sscanf(slist,"x=%d y=%d dir=[%d %d] e=%f\n", &new_x, &new_y, &new_dir[0], &new_dir[1], &e_lu);
        Animal *new_a=creer_animal(new_x, new_y, e_lu);
        new_a->dir[0]=new_dir[0];
        new_a->dir[1]=new_dir[1];
        //inserting at the top of the list
        new_a->suivant=*liste_predateur;
        *liste_predateur = new_a;
        fgets(slist, 256, f); //reading the line containing the next predator
    }

    fclose(f);
}

void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
  //unsigned int i, j;
  int i,j;
  char ecosys[SIZE_Y][SIZE_X];
  Animal *pa=NULL;

  /* on initialise le tableau */
  for (i = 0; i < SIZE_Y; ++i) {
    for (j = 0; j < SIZE_X; ++j) {
      ecosys[i][j]=' ';
    }
  }

  /* on ajoute les proies */
  pa = liste_proie;
  while (pa) {
    ecosys[pa->x][pa->y] = '*';
    pa=pa->suivant;
  }

  /* on ajoute les predateurs */
  pa = liste_predateur;
  while (pa) {
      if ((ecosys[pa->x][pa->y] == '@') || (ecosys[pa->x][pa->y] == '*')) { /* proies aussi present */
        ecosys[pa->x][pa->y] = '@';
      } else {
        ecosys[pa->x][pa->y] = 'O';
      }
    pa = pa->suivant;
  }

  /* on affiche le tableau */
  printf("+");
  for (j = 0; j < SIZE_X; ++j) {
    printf("-");
  }  
  printf("+\n");
  for (i = 0; i < SIZE_Y; ++i) {
    printf("|");
    for (j = 0; j < SIZE_X; ++j) {
      putchar(ecosys[i][j]);
    }
    printf("|\n");
  }
  printf("+");
  for (j = 0; j<SIZE_X; ++j) {
    printf("-");
  }
  printf("+\n");
  int nbproie=compte_animal_it(liste_proie);
  int nbpred=compte_animal_it(liste_predateur);
  
  printf("Nb proies : %5d\tNb predateurs : %5d\n", nbproie, nbpred);

}


// void clear_screen() {
//   printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 pour effacer l'ecran */
// }

// /* PARTIE 2*/

// /* Part 2. Exercice 4, question 1 */
// void bouger_animaux(Animal *la) {
//   Animal *ap = la;
//   while (ap) {
//       if (rand() / (float)RAND_MAX < p_ch_dir) {  // changement de direction avec une probabilité p_ch_dir
//           ap->dir[0] = rand() % 3 - 1;
//           ap->dir[1] = rand() % 3 - 1;
//       }
//       // déplacer l'animal selon sa direction
//       // Et vérifiez que les nouvelles coordonnées de l'animal sont bien dans les limites de taille du tableau
//       ap->x = (ap->x + ap->dir[0] + SIZE_X) % SIZE_X; 
//       ap->y = (ap->y + ap->dir[1] + SIZE_Y) % SIZE_Y;
//       ap = ap->suivant;
//   }

// }

// /* Part 2. Exercice 4, question 3 */
// void reproduce(Animal **liste_animal, float p_reproduce) {
//   Animal *ap = liste_animal!= NULL ? *liste_animal : NULL;
//   while (ap) {
//       if (rand() / (float)RAND_MAX < p_reproduce) {  // reproduction d'un animal avec probabilité p_reproduce
//           ajouter_animal(ap->x, ap->y, ap->energie/2, liste_animal); // créer un nouvel animal avec la moitié de l'énergie de son parent
//           ap->energie=ap->energie/2;// diviser énergie de parent par 2
//       }
//       ap = ap->suivant;
//   }

// }


// /* Part 2. Exercice 6, question 1 */
// void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) {
//     /*A Completer*/

// }

// /* Part 2. Exercice 7, question 1 */
// Animal *animal_en_XY(Animal *l, int x, int y) {
//     /*A Completer*/

//   return NULL;
// } 

// /* Part 2. Exercice 7, question 2 */
// void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie) {
//    /*A Completer*/

// }

// /* Part 2. Exercice 5, question 2 */
// void rafraichir_monde(int monde[SIZE_X][SIZE_Y]){

//    /*A Completer*/


// }

