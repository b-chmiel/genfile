import subprocess
import os

RUNS=100
FILE="FILE"

for i in range(0, RUNS):
    print(f"{i} / {RUNS}")
    subprocess.run(["./build/src/gen_file", "--size=10M", "--seed=420", "FILE"])
    assert(os.path.exists(FILE))
    subprocess.run(["rm", FILE])
