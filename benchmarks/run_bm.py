import subprocess
import re
import csv
import os

os.makedirs("reports", exist_ok=True)

sizes = [500, 5000, 10000, 20000, 30000]
nb_threads = 8
results = []

for n in sizes:
    cmd = ["bin/matmul", f"benchmarks/matA_{n}.txt", f"benchmarks/matB_{n}.txt", str(nb_threads)]
    print(f"Execution pour {n}x{n}...")
    proc = subprocess.run(cmd, capture_output=True, text=True)
    output = proc.stdout

    t_seq = float(re.search(r"Multiplication sequentielle : ([0-9.eE+-]+) s", output).group(1))
    t_par = float(re.search(r"Multiplication parallèle .* : ([0-9.eE+-]+) s", output).group(1))
    speedup = t_seq / t_par if t_par != 0 else 0.0

    results.append([n, t_seq, t_par, speedup])
    print(f"Résultat {n}x{n} -> seq: {t_seq:.6f}s, par: {t_par:.6f}s, speedup: {speedup:.2f}x")

with open("reports/times.csv", "w", newline="") as f:
    writer = csv.writer(f)
    writer.writerow(["Size", "Time_seq_s", "Time_par_s", "Speedup"])
    writer.writerows(results)

print("Benchmark terminé. Résultats dans reports/times.csv")
