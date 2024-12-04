#include <string>
#include "JeuDeLaVie.h"

int main() {
    std::string fichierEntree = "input.txt";
    std::string fichierSortie = "resultat.txt";
    int nombreIterations = 100;

    JeuDeLaVie jeu(fichierEntree, nombreIterations);
    jeu.executer(fichierSortie);

    return 0;
}
