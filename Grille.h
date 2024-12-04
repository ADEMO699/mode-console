#ifndef GRILLE_H
#define GRILLE_H

#include <vector>
#include <string>
#include "Cellule.h"

class Grille {
private:
    std::vector<std::vector<Cellule>> cellules;
    int largeur;
    int hauteur;

public:
    Grille(int largeur, int hauteur);

    void chargerDepuisFichier(const std::string& cheminFichier);
    void sauvegarderDansFichier(const std::string& cheminFichier) const;
    void afficher() const;
    int compterVoisinsVivants(int x, int y) const;
    void miseAJour();
};

#endif // GRILLE_H
