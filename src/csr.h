#ifndef CSR_H
#define CSR_H

typedef struct{
    int lignes;
    int colonnes;
    int nb_vnn; // Nombre de valeurs non nulles
    double* valeurs; // Il va contenir les valeurs non nulles
    int* index_colonnes; // Il va contenir l'indice de la colonne associé à chaque valeur
    int* ptr_lignes; // Il nous donne l'info sur où commence chaque ligne dans valeurs
} spMatrice_t;

/*
Format attendu du fichier.txt pour chargement :

nb_lignes nb_colonnes nb_vnn
i         j           valeur
...       ...         ...

Afficher la matrice CSR :
param mat : pointeur vers la matrice CSR
param format : 0 = afficher tableaux CSR, 1 = affichage matrice classique
*/

spMatrice_t* charger_matrice_txt(const char* chemin);
void afficher_matrice(spMatrice_t *mat, int format);
void free_matrice(spMatrice_t *mat);

#endif