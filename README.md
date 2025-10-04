# Multiplication des Matrices creuses – Séquentielle & Parallèle (OpenMP)

## Description

Ce projet implémente la **multiplication de matrices creuses** en utilisant le format  **CSR (Compressed Sparse Row)** .

Il propose deux versions :

* **Séquentielle** : algorithme simple de multiplication CSR × CSR.
* **Parallèle** : utilise **OpenMP** pour paralléliser la multiplication par lignes.

Le projet est écrit en  **C** , avec un focus sur la **performance** pour analyser l’efficacité de la parallélisation.

---

## Compilation

Assurez-vous que `gcc-15` est installé et supporte OpenMP.

```bash
make
```

* Le binaire compilé sera créé dans :

```
bin/matmul
```

* Options dans le Makefile : optimisation `-O2` et OpenMP activé.

---

## Exécution

```bash
./bin/matmul <chemin_matA> <chemin_matB> <nb_threads>
```

* `<chemin_matA>` : fichier texte de la matrice A (format CSR : `lignes colonnes nb_valeurs`, suivi des valeurs).
* `<chemin_matB>` : fichier texte de la matrice B.
* `<nb_threads>` : nombre de threads OpenMP pour la version parallèle.

  *NB : Dans le dossier benchmarks, il y a un script pour exécuter automatiquement le jeu de données. Le nombre de threads peut etre modifié directement à partir du script. (Étudier le script pour comprendre comment il exécute).*

**Exemple** :

```bash
./bin/matmul benchmarks/matA_5000.txt benchmarks/matB_5000.txt 4
```

* Le programme affiche :
  * Dimensions des matrices
  * Temps de multiplication séquentielle
  * Temps de multiplication parallèle
  * Speedup
  * Vérification de l’égalité des matrices résultats
  * Affichage optionnel de la matrice résultat.

---

## Résultats des produits

Les benchmarks ont été effectués sur des matrices carrées creuses de tailles différentes avec sparsité 0.1%.

| Size  | Time_seq_s | Time_par_s | Speedup |
| ----- | ---------- | ---------- | ------- |
| 500   | 0.000151   | 0.000276   | 0.55x   |
| 5000  | 0.014181   | 0.008088   | 1.75x   |
| 10000 | 0.058587   | 0.046541   | 1.26x   |
| 20000 | 0.266370   | 0.104563   | 2.55x   |
| 30000 | 0.675277   | 0.238561   | 2.83x   |

---

## Analyse

1. **Exactitude**
   * Les résultats séquentiels et parallèles sont identiques (`Matrices identiques : OUI`) pour toutes les tailles testées.
2. **Performance**
   * Pour les petites matrices (500×500), le  **speedup < 1** , la parallélisation n’est pas efficace car le  **coût de gestion des threads dépasse le temps de calcul** .
   * À partir de matrices 5000×5000 et plus, on observe un **gain significatif** (speedup > 1).
   * Les grandes matrices profitent pleinement du parallélisme OpenMP.
3. **Tendance**
   * Le speedup augmente avec la taille des matrices, mais reste limité par :
     * **Nombre de threads disponibles**
     * **Mémoire et accès aux tableaux temporaires**
4. **Observations**
   * Pour des matrices très grandes (>100000×100000), la mémoire devient critique.
   * Le projet montre comment **la multiplication creuse peut être optimisée** pour HPC tout en restant correcte et reproductible.

---

## Conclusion

* La multiplication de matrices creuses en **CSR** est **efficace et scalable** pour de grandes matrices.
* OpenMP permet d’obtenir des gains réels à partir d’une taille de matrice suffisante.
* Ce mini projet constitue une **base solide pour l’étude de la parallélisation et du profilage** sur des algorithmes scientifiques.
