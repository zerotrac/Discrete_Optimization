import numpy as np


def clean_data_ks_400():
    f = open("./data/ks_400_0")
    n, k = [int(x) for x in f.readline().strip().split()]
    v, w = np.zeros(n, dtype=np.int64), np.zeros(n, dtype=np.int64)

    for i in range(n):
        v[i], w[i] = [int(x) for x in f.readline().strip().split()]
        if v[i] / w[i] < 0.1:
            print(v[i], w[i])
    
    price = v / w
    print(np.min(price), np.max(price))


def clean_data_ks_10000():
    f = open("./data/ks_10000_0")
    n, k = [int(x) for x in f.readline().strip().split()]
    v, w = np.zeros(n, dtype=np.int64), np.zeros(n, dtype=np.int64)

    for i in range(n):
        v[i], w[i] = [int(x) for x in f.readline().strip().split()]
    
    price = v / w
    print(np.min(price), np.max(price))


if __name__ == "__main__":
    clean_data_ks_400()
    clean_data_ks_10000()
