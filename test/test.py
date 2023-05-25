import subprocess
import os

FILE="FILE"

def genfile(size):
    subprocess.run(["./build/src/genfile", f"--size={size}", "--seed=420", FILE])

def validate(size):
    assert(os.path.exists(FILE))
    assert(os.stat(FILE).st_size == size)

def test_smaller_than_chunk():
    size = 1024
    genfile(size)
    validate(size)
    subprocess.run(["rm", FILE])


def test_multiple_chunks():
    size = 2048 ** 2
    genfile(size)
    validate(size)
    subprocess.run(["rm", FILE])

def test_not_divisible_by_chunks():
    size = 12345678
    genfile(size)
    validate(size)
    subprocess.run(["rm", FILE])
    
def test_repeated():
    RUNS=100
    for i in range(0, RUNS):
        size = 1024
        genfile(size)
        validate(size)
        subprocess.run(["rm", FILE])

# def test_repeated():
#     RUNS=100
#     for i in range(0, RUNS):
#         print(f"{i} / {RUNS}")
#         size = 12345678
#         genfile(size)
#         validate(size)
#         subprocess.run(["rm", FILE])
