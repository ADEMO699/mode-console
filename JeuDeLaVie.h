#ifndef JEUDELAVIE_H
#define JEUDELAVIE_H

#include <string>
#include "Grille.h"

class JeuDeLaVie {
private:
    Grille grille;
    int iterationsMax;

public:
    JeuDeLaVie(const std::string& cheminFichier, int iterations);

    void executer(const std::string& fichierSortie);
};

#endif // JEUDELAVIE_H
