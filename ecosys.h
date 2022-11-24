#define SIZE_X = 20
#define SIZE_Y = 50

typedef struct animal{
    int dir[2];
    int x,y;
    float energie;
    struct animal *suivant;
} Animal;

Animal *creer_animal(int x, int y, float energie);

Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal);

unsigned int compte_animal_it(Animal *la);

unsigned int compte_animal_rec(Animal *la);

void ajouter_animal(int x, int y,  float energie, Animal **liste_animal);

void enlever_animal(Animal **liste, Animal *animal);

Animal* liberer_liste_animaux(Animal *liste);

void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur);

void clear_screen();

void bouger_animaux(Animal *la);