import numpy as np

def generer_matrice_creuse(fichier, lignes, colonnes, vnn):
    mat = {}
    for _ in range(vnn):
        i = np.random.randint(0, lignes)
        j = np.random.randint(0, colonnes)
        val = round(np.random.uniform(1, 10), 2)
        mat[(i, j)] = val  
    with open(fichier, 'w') as f:
        f.write(f"{lignes} {colonnes} {len(mat)}\n")
        for (i,j), val in mat.items():
            f.write(f"{i} {j} {val}\n")

n = 5000
vnn = int(n * n * 0.001)

generer_matrice_creuse("data/matA1.txt", n, n, vnn)
generer_matrice_creuse("data/matB2.txt", n, n, vnn)