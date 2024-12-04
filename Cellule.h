#ifndef CELLULE_H
#define CELLULE_H

class Cellule {
private:
    bool vivante;

public:
    Cellule(bool etat = false);

    bool estVivante() const;
    void setVivante(bool etat);
};

#endif // CELLULE_H

