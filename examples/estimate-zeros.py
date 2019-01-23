# Estimate the zeros of a noisy sinusoid using a polynomial fit

import numpy as np

if __name__ == "__main__":
    x = np.linspace(-0.1, 2.1, 100) * np.pi
    y = np.sin(x) + 0.1 * np.random.rand(100)

    P = np.polynomial.polynomial.polyfit(x, y, 3)
    zeros = np.polynomial.polynomial.polyroots(P)

    print("Zeros: Expect 0, pi, 2 pi")
    print zeros