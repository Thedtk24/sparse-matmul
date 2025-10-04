#include <stdio.h>
#include <stdlib.h>
#include "csr.h"

spMatrice_t* charger_matrice_txt(const char* chemin){
    FILE *f = fopen(chemin, "r");
    if(!f){
        perror("Erreur d'ouverture du fichier txt");
        exit(EXIT_FAILURE);
    }

    spMatrice_t *mat = malloc(sizeof(spMatrice_t));
    if(!mat){
        perror("Erreur d'allocation de la matrice");
        fclose(f);
        exit(EXIT_FAILURE);
    }

    if(fscanf(f, "%d %d %d", &mat->lignes, &mat->colonnes, &mat->nb_vnn) != 3){
        fprintf(stderr, "Le format de fichier est invalide.\n");
        fclose(f);
        free(mat);
        exit(EXIT_FAILURE);
    }

    mat -> valeurs  = malloc(mat -> nb_vnn * sizeof(double));
    mat -> index_colonnes = malloc(mat -> nb_vnn * sizeof(int));
    mat -> ptr_lignes = calloc(mat -> lignes + 1, sizeof(int));

    if(!mat -> valeurs || !mat -> index_colonnes || !mat -> ptr_lignes){
        fprintf(stderr, "Erreur d'allocation du tableaux.\n");
        fclose(f);
        free(mat -> valeurs); free(mat -> index_colonnes); free(mat -> ptr_lignes); free(mat);
        exit(EXIT_FAILURE);
    }

    int *lignes_compt = calloc(mat -> lignes, sizeof(int));
    if(!lignes_compt){
        fprintf(stderr, "Erreur d'allocation compteur lignes.\n");
        fclose(f);
        free(mat -> valeurs); free(mat -> index_colonnes); free(mat -> ptr_lignes); free(mat);
        exit(EXIT_FAILURE);
    }

    int i, j;
    double val;
    long pos = ftell(f);

    for(int k = 0; k < mat -> nb_vnn; k++){
        if(fscanf(f, "%d %d %lf", &i, &j, &val) != 3){
            fprintf(stderr, "Erreur lecture de la ligne %d.\n", k + 1);
            exit(EXIT_FAILURE);
        }
        if(i < 0 || i >= mat -> lignes){
            fprintf(stderr, "Indice de ligne invalide : %d\n", i);
            exit(EXIT_FAILURE);
        }
        lignes_compt[i]++;
    }

    mat -> ptr_lignes[0] = 0;
    for(int r = 0; r < mat -> lignes; r++){
        mat -> ptr_lignes[r + 1] = mat -> ptr_lignes[r] + lignes_compt[r];
    }

    rewind(f);
    fscanf(f, "%d %d %d", &mat -> lignes, &mat -> colonnes, &mat -> nb_vnn); 

    int *position_ligne = calloc(mat -> lignes, sizeof(int));
    if(!position_ligne){
        fprintf(stderr, "Erreur allocation position de la ligne\n");
        exit(EXIT_FAILURE);
    }

    for(int k = 0; k < mat -> nb_vnn; k++){
        if(fscanf(f, "%d %d %lf", &i, &j, &val) != 3){
            fprintf(stderr, "Erreur de lecture de la ligne %d.\n", k + 1);
            exit(EXIT_FAILURE);
        }
        int index = mat -> ptr_lignes[i] + position_ligne[i];
        mat -> valeurs[index] = val;
        mat -> index_colonnes[index] = j;
        position_ligne[i]++;
    }


    fclose(f);
    free(lignes_compt);
    free(position_ligne);

    return mat;
}

void afficher_matrice(spMatrice_t *mat, int format){
    if(!mat){
        printf("Matrice NULL.\n");
        return;
    }

    if(format == 0){
        printf("=== Matrice CSR brute ===\n");
        printf("valeurs : ");
        for(int i = 0; i < mat -> nb_vnn; i++)
            printf("%.2f ", mat -> valeurs[i]);
        printf("\nindex_colonnes: ");
        for(int i = 0; i < mat -> nb_vnn; i++)
            printf("%d ", mat -> index_colonnes[i]);
        printf("\nptr_lignes: ");
        for(int i = 0; i <= mat -> lignes; i++)
            printf("%d ", mat -> ptr_lignes[i]);
        printf("\n========================\n");
    } else {
        printf("=== Matrice construite ===\n");
        for(int i = 0; i < mat -> lignes; i++){
            int debut = mat -> ptr_lignes[i];
            int fin   = mat -> ptr_lignes[i+1];
            int pos = debut;
            for(int j = 0; j < mat -> colonnes; j++){
                if(pos < fin && mat -> index_colonnes[pos] == j){
                    printf("%6.2f ", mat -> valeurs[pos]);
                    pos++;
                }else{
                    printf("%6.2f ", 0.0);
                }
            }
            printf("\n");
        }
        printf("===============================\n");
    }
}

void free_matrice(spMatrice_t *mat){
    if (!mat) return; 
    if (mat -> valeurs) free(mat -> valeurs);
    if (mat -> index_colonnes) free(mat -> index_colonnes);
    if (mat -> ptr_lignes) free(mat -> ptr_lignes);
    free(mat);
}