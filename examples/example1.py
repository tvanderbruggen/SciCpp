import numpy as np

if __name__ == "__main__":
    a = np.linspace(0.0, 10.0, 100)
    print(a)
    print("Mean {}".format(np.mean(a)))
    print("Var {}\n".format(np.var(a)))
    
    v = np.linspace(0.0, 10.0, 100000000)
    print(v)
    print("Mean {}".format(np.mean(v)))
    print("Var {}\n".format(np.var(v)))
