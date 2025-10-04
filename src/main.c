#include "csr.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

double difference_temps(struct timespec debut, struct timespec fin){
    return (fin.tv_sec - debut.tv_sec) + 1e-9*(fin.tv_nsec - debut.tv_nsec);
}

spMatrice_t* csr_mult_seq(spMatrice_t* matA, spMatrice_t* matB);
spMatrice_t* csr_mult_par(spMatrice_t* matA, spMatrice_t* matB, int nb_threads);


int main(int argc, char *argv[]){
    if(argc < 4){
        printf("Usage: %s matA.txt matB.txt nb_threads\n", argv[0]);
        return 1;
    }

    const char *fileA = argv[1];
    const char *fileB = argv[2];
    int nb_threads = atoi(argv[3]);

    spMatrice_t* matA = charger_matrice_txt(fileA);
    spMatrice_t* matB = charger_matrice_txt(fileB);

    printf("Matrices chargees : matA(%dx%d) matB(%dx%d)\n", matA -> lignes, matA -> colonnes, matB -> lignes, matB -> colonnes);

    /* Multiplication séquentielle */
    struct timespec debut, fin;
    clock_gettime(CLOCK_MONOTONIC, &debut);
    spMatrice_t *matC_seq = csr_mult_seq(matA, matB);
    clock_gettime(CLOCK_MONOTONIC, &fin);
    double temps_seq = difference_temps(debut, fin);
    printf("Multiplication sequentielle : %.6f s\n", temps_seq);

    /* Multiplication parallèle */
    clock_gettime(CLOCK_MONOTONIC, &debut);
    spMatrice_t *matC_par = csr_mult_par(matA, matB, nb_threads);
    clock_gettime(CLOCK_MONOTONIC, &fin);
    double temps_par = difference_temps(debut, fin);
    printf("Multiplication parallèle (%d threads) : %.6f s\n", nb_threads, temps_par);

    double speedup = temps_seq / temps_par;
    printf("Speedup : %.2fx\n", speedup);

    int egal = 1;
    if(matC_seq -> nb_vnn != matC_par -> nb_vnn) egal = 0;
    else{
        for(int i = 0; i < matC_seq -> nb_vnn; i++){
            if(matC_seq -> valeurs[i] != matC_par -> valeurs[i] ||
                matC_seq -> index_colonnes[i] != matC_par -> index_colonnes[i]){
                egal = 0;
                break;
            }
        }
    }
    printf("Matrices identiques : %s\n", egal ? "OUI" : "NON");

    if (matC_seq -> lignes <= 10 && matC_seq -> colonnes <= 10) {
        afficher_matrice(matC_seq, 1);
    }

    free_matrice(matA);
    free_matrice(matB);
    free_matrice(matC_seq);
    free_matrice(matC_par);

    return 0;
}