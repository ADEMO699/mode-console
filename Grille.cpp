#include "Grille.h"
#include <iostream>
#include <fstream>

Grille::Grille(int largeur, int hauteur) : largeur(largeur), hauteur(hauteur) {
    cellules.resize(hauteur, std::vector<Cellule>(largeur));
}

void Grille::chargerDepuisFichier(const std::string& cheminFichier) {
    std::ifstream fichier(cheminFichier);

    if (!fichier.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << cheminFichier << std::endl;
        exit(1);
    }

    fichier >> hauteur >> largeur;
    cellules.resize(hauteur, std::vector<Cellule>(largeur));

    for (int i = 0; i < hauteur; ++i) {
        for (int j = 0; j < largeur; ++j) {
            int etat;
            fichier >> etat;
            cellules[i][j].setVivante(etat == 1);
        }
    }

    fichier.close();
}

void Grille::sauvegarderDansFichier(const std::string& cheminFichier) const {
    std::ofstream fichier(cheminFichier);

    if (!fichier.is_open()) {
        std::cerr << "Erreur : Impossible d'ouvrir le fichier " << cheminFichier << " pour écrire." << std::endl;
        exit(1);
    }

    fichier << hauteur << " " << largeur << "\n";
    for (const auto& ligne : cellules) {
        for (const auto& cellule : ligne) {
            fichier << (cellule.estVivante() ? 1 : 0) << " ";
        }
        fichier << "\n";
    }

    fichier.close();
}

void Grille::afficher() const {
    for (const auto& ligne : cellules) {
        for (const auto& cellule : ligne) {
            std::cout << (cellule.estVivante() ? "1 " : "0 ");
        }
        std::cout << "\n";
    }
}

int Grille::compterVoisinsVivants(int x, int y) const {
    int voisinsVivants = 0;

    for (int dx = -1; dx <= 1; ++dx) {
        for (int dy = -1; dy <= 1; ++dy) {
            if (dx == 0 && dy == 0) continue;

            int nx = x + dx;
            int ny = y + dy;

            if (nx >= 0 && nx < hauteur && ny >= 0 && ny < largeur) {
                voisinsVivants += cellules[nx][ny].estVivante();
            }
        }
    }

    return voisinsVivants;
}

void Grille::miseAJour() {
    std::vector<std::vector<Cellule>> nouvelleGrille = cellules;

    for (int i = 0; i < hauteur; ++i) {
        for (int j = 0; j < largeur; ++j) {
            int voisinsVivants = compterVoisinsVivants(i, j);

            if (cellules[i][j].estVivante()) {
                if (voisinsVivants < 2 || voisinsVivants > 3) {
                    nouvelleGrille[i][j].setVivante(false);
                }
            }
            else {
                if (voisinsVivants == 3) {
                    nouvelleGrille[i][j].setVivante(true);
                }
            }
        }
    }

    cellules = nouvelleGrille;
}

