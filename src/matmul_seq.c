#include <stdio.h>
#include <stdlib.h>
#include "csr.h"

spMatrice_t* csr_mult_seq(spMatrice_t* matA, spMatrice_t* matB){
    if(matA -> colonnes != matB -> lignes){
        fprintf(stderr, "Erreur : Les dimensions sont incompatibles pour la multiplication.\n");
        return NULL;
    }

    spMatrice_t *matC = malloc(sizeof(spMatrice_t));

    matC -> lignes = matA -> lignes;
    matC -> colonnes = matB ->colonnes;
    matC -> nb_vnn = 0; 
    matC -> ptr_lignes = calloc(matC -> lignes + 1, sizeof(int));

    int taille_max = matA -> nb_vnn * matB -> nb_vnn; 
    matC -> valeurs = malloc(taille_max * sizeof(double));
    matC -> index_colonnes = malloc(taille_max * sizeof(int));

    double *temp = calloc(matB -> colonnes, sizeof(double)); 
    int *flag = calloc(matB -> colonnes, sizeof(int)); 
    int vnn_ligne;

    for(int i = 0; i < matA -> lignes; i++){
        vnn_ligne = 0; 

        for(int k = matA -> ptr_lignes[i]; k < matA -> ptr_lignes[i+1]; k++){
            int col_A = matA -> index_colonnes[k];
            double val_A = matA -> valeurs[k];

            for(int l = matB -> ptr_lignes[col_A]; l < matB -> ptr_lignes[col_A + 1]; l++){
                int col_B = matB -> index_colonnes[l];
                double val_B = matB -> valeurs[l];

                if(flag[col_B] == 0){ 
                    flag[col_B] = 1;
                    vnn_ligne++; 
                }
                temp[col_B] += val_A * val_B;
            }
        }

        for(int j = 0; j < matB -> colonnes; j++){
            if(flag[j]){
                //int index = matC -> ptr_lignes[i] + (matC -> nb_vnn - matC -> ptr_lignes[i]);
                matC -> valeurs[matC -> nb_vnn] = temp[j];
                matC -> index_colonnes[matC -> nb_vnn] = j;
                matC -> nb_vnn++;
                temp[j] = 0.0;  
                flag[j] = 0;    
            }
        }

        matC -> ptr_lignes[i + 1] = matC -> nb_vnn;
    }

    free(temp);
    free(flag);

    return matC;
}