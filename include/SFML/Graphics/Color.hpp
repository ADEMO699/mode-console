#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <chrono>

// Classe représentant une cellule
class Cellule {
private:
    bool vivante;

public:
    Cellule(bool etat = false) : vivante(etat) {}

    bool estVivante() const { return vivante; }
    void setVivante(bool etat) { vivante = etat; }
};

// Classe représentant la grille
class Grille {
private:
    std::vector<std::vector<Cellule>> cellules;
    int largeur;
    int hauteur;

public:
    Grille(int largeur, int hauteur) : largeur(largeur), hauteur(hauteur) {
        cellules.resize(hauteur, std::vector<Cellule>(largeur));
    }

    void chargerDepuisFichier(const std::string& cheminFichier) {
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

    void sauvegarderDansFichier(const std::string& cheminFichier) const {
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

    void afficherConsole() const {
        for (const auto& ligne : cellules) {
            for (const auto& cellule : ligne) {
                std::cout << (cellule.estVivante() ? "1 " : "0 ");
            }
            std::cout << "\n";
        }
    }

    int compterVoisinsVivants(int x, int y) const {
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

    void miseAJour() {
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

    void afficherGraphique(sf::RenderWindow& fenetre, int tailleCellule) const {
        fenetre.clear(sf::Color::Black);

        for (int i = 0; i < hauteur; ++i) {
            for (int j = 0; j < largeur; ++j) {
                sf::RectangleShape cellule(sf::Vector2f(tailleCellule - 1, tailleCellule - 1));
                cellule.setPosition(j * tailleCellule, i * tailleCellule);
                if (cellules[i][j].estVivante()) {
                    cellule.setFillColor(sf::Color::White);
                }
                else {
                    cellule.setFillColor(sf::Color::Black);
                }
                fenetre.draw(cellule);
            }
        }

        fenetre.display();
    }
};

// Classe principale du jeu
class JeuDeLaVie {
private:
    Grille grille;
    int iterationsMax;

public:
    JeuDeLaVie(const std::string& cheminFichier, int iterations) : grille(0, 0), iterationsMax(iterations) {
        grille.chargerDepuisFichier(cheminFichier);
    }

    void executerConsole(const std::string& fichierSortie) {
        std::cout << "État initial :\n";
        grille.afficherConsole();

        for (int it = 0; it < iterationsMax; ++it) {
            std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Pause de 500ms entre chaque itération
            std::cout << "\n--- Itération " << it + 1 << " ---\n";
            grille.miseAJour();
            grille.afficherConsole();
        }

        std::cout << "\nSimulation terminée après " << iterationsMax << " itérations.\n";
        grille.sauvegarderDansFichier(fichierSortie);
    }

    void executerGraphique(int tailleCellule) {
        sf::RenderWindow fenetre(sf::VideoMode(800, 600), "Jeu de la Vie");

        while (fenetre.isOpen()) {
            sf::Event event;
            while (fenetre.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    fenetre.close();
                }
            }

            grille.afficherGraphique(fenetre, tailleCellule);
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            grille.miseAJour();
        }
    }
};

int main() {
    std::string fichierEntree = "input.txt";
    std::string fichierSortie = "resultat.txt";
    int nombreIterations = 10;

    std::cout << "Choisissez le mode d'exécution :\n";
    std::cout << "1. Mode console\n";
    std::cout << "2. Mode graphique\n";
    int choix;
    std::cin >> choix;

    JeuDeLaVie jeu(fichierEntree, nombreIterations);

    if (choix == 1) {
        jeu.executerConsole(fichierSortie);
    }
    else if (choix == 2) {
        jeu.executerGraphique(20);
    }
    else {
        std::cerr << "Choix invalide.\n";
    }

    return 0;
}
