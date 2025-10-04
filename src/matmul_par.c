#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "csr.h"

spMatrice_t* csr_mult_par(spMatrice_t* matA, spMatrice_t* matB, int nb_threads){
    if(matA->colonnes != matB->lignes){
        fprintf(stderr, "Erreur : dimensions incompatibles.\n");
        return NULL;
    }

    spMatrice_t *matC = malloc(sizeof(spMatrice_t));
    matC->lignes = matA->lignes;
    matC->colonnes = matB->colonnes;
    matC->nb_vnn = 0;
    matC->ptr_lignes = calloc(matC->lignes + 1, sizeof(int));

    // 1️⃣ Compter le nombre maximum de nnz par ligne (séquentiel)
    int *vnn_ligne_arr = calloc(matC->lignes, sizeof(int));
    
    #pragma omp parallel for num_threads(nb_threads)
    for(int i = 0; i < matA->lignes; i++){
        int vnn_ligne = 0;
        int *flag = calloc(matB->colonnes, sizeof(int));

        for(int k = matA->ptr_lignes[i]; k < matA->ptr_lignes[i+1]; k++){
            int col_A = matA->index_colonnes[k];

            for(int l = matB->ptr_lignes[col_A]; l < matB->ptr_lignes[col_A+1]; l++){
                int col_B = matB->index_colonnes[l];
                if(flag[col_B] == 0){
                    flag[col_B] = 1;
                    vnn_ligne++;
                }
            }
        }
        vnn_ligne_arr[i] = vnn_ligne;
        free(flag);
    }

    // 2️⃣ Construire ptr_lignes et calculer nb_vnn total
    matC->ptr_lignes[0] = 0;
    for(int i = 0; i < matC->lignes; i++){
        matC->ptr_lignes[i+1] = matC->ptr_lignes[i] + vnn_ligne_arr[i];
    }
    matC->nb_vnn = matC->ptr_lignes[matC->lignes];

    // 3️⃣ Allouer valeurs et index_colonnes
    matC->valeurs = calloc(matC->nb_vnn, sizeof(double));
    matC->index_colonnes = malloc(matC->nb_vnn * sizeof(int));

    // 4️⃣ Remplir valeurs et index_colonnes (parallèle)
    #pragma omp parallel for num_threads(nb_threads)
    for(int i = 0; i < matA->lignes; i++){
        double *temp = calloc(matB->colonnes, sizeof(double));
        int *flag = calloc(matB->colonnes, sizeof(int));

        for(int k = matA->ptr_lignes[i]; k < matA->ptr_lignes[i+1]; k++){
            int col_A = matA->index_colonnes[k];
            double val_A = matA->valeurs[k];

            for(int l = matB->ptr_lignes[col_A]; l < matB->ptr_lignes[col_A+1]; l++){
                int col_B = matB->index_colonnes[l];
                double val_B = matB->valeurs[l];
                if(flag[col_B] == 0) flag[col_B] = 1;
                temp[col_B] += val_A * val_B;
            }
        }

        int idx = matC->ptr_lignes[i];
        for(int j = 0; j < matB->colonnes; j++){
            if(flag[j]){
                matC->valeurs[idx] = temp[j];
                matC->index_colonnes[idx] = j;
                idx++;
            }
        }

        free(temp);
        free(flag);
    }

    free(vnn_ligne_arr);
    return matC;
}
