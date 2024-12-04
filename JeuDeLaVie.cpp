#include "JeuDeLaVie.h"
#include <iostream>
#include <thread>
#include <chrono>

JeuDeLaVie::JeuDeLaVie(const std::string& cheminFichier, int iterations)
    : grille(0, 0), iterationsMax(iterations) {
    grille.chargerDepuisFichier(cheminFichier);
}

void JeuDeLaVie::executer(const std::string& fichierSortie) {
    std::cout << "État initial :\n";
    grille.afficher();

    for (int it = 0; it < iterationsMax; ++it) {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        std::cout << "\n--- Itération " << it + 1 << " ---\n";
        grille.miseAJour();
        grille.afficher();
    }

    std::cout << "\nSimulation terminée après " << iterationsMax << " itérations.\n";
    grille.sauvegarderDansFichier(fichierSortie);
}

