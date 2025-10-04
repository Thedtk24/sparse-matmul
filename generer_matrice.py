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

generer_matrice_creuse("tests/matA1.txt", n, n, vnn)
generer_matrice_creuse("tests/matB2.txt", n, n, vnn)

sizes = [500, 5000, 10000, 20000, 30000]
sparsity = 0.001  # 0.1% non-null

for e in sizes:
    vnn2 = int(e * e * sparsity)
    generer_matrice_creuse(f"benchmarks/matA_{e}.txt", e, e, vnn2)
    generer_matrice_creuse(f"benchmarks/matB_{e}.txt", e, e, vnn2)
    print(f"Matrices {e}x{e} générées avec {vnn2} valeurs non nulles")