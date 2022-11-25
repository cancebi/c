#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#define SIZE_X 40
#define SIZE_Y 40

extern float p_ch_dir; //probabilite de changer de direction de deplacement
extern float p_reproduce_proie;
extern float p_reproduce_predateur;
extern int temps_repousse_herbe;

typedef struct _Animal Animal;
struct _Animal{
    int x;
    int y;
    float energie;
    int dir[2];
    Animal* suivant;
};

Animal* creer_animal(int x, int y, float energie);

Animal* ajouter_en_tete_animal(Animal* liste, Animal* animal);

unsigned int compte_animal_rec(Animal* la);

unsigned int compte_animal_it(Animal* la);

void init_ecosysteme(char eco[SIZE_X][SIZE_Y]);

void remplir_ecosysteme(char eco[SIZE_X][SIZE_Y], Animal* liste_proies, Animal* liste_predateurs);

void afficher_ecosysteme(char tab[SIZE_X][SIZE_Y], Animal* liste_proies, Animal* liste_predateurs);

void ajouter_animal(int x, int y, Animal** liste_animal);

Animal* liberer_liste_animaux(Animal* liste);

void enlever_animal(Animal** liste, Animal* animal);

void ecrire_ecosys(const char* nom_fichier, Animal* liste_predateur, Animal* liste_proie);

void lire_ecosys(const char* nom_fichier, Animal** liste_predateur, Animal** liste_proie);

void bouger_animaux(Animal* la);

void reproduce(Animal** liste_animal, float p_reproduce);

void rafraichir_proies(Animal** liste_proie, int monde[SIZE_X][SIZE_Y]);

Animal* animal_en_XY(Animal* l, int x, int y);

void rafraichir_predateurs(Animal** liste_predateur, Animal** liste_proie);

void rafraichir_monde(int monde[SIZE_X][SIZE_Y]);