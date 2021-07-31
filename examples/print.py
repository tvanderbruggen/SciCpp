# SPDX-License-Identifier: MIT
# Copyright (c) 2019-2021 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

import numpy as np
from scipy.signal import hann

if __name__ == "__main__":
    # ---- Print a long vector
    v = np.linspace(0., 10., 10000000)
    print(v)

    # ---- Print a short array of reals
    window = hann(16)
    print(window)
 
    #  ---- Print a short array of complex
    phi = np.linspace(0., 2. * np.pi, window.size)
    window_cplx = window * np.exp(1j * phi)
    print(window_cplx)

    # ---- Print a short array of tuples
    atup = np.zeros(10, dtype=[('1', 'int32'), ('2', 'int8'), ('3', 'float64')])

    for i in range(atup.size):
        atup[i] = (i, i % 2, 3.14 * i * i)
    
    print(atup)

    # ---- Print a long array of tuples

    vtup = np.zeros(10000000, dtype=[('1', 'int32'), ('2', 'int8'), ('3', 'float64')])

    for i in range(vtup.size):
       vtup[i] = (i, i % 2, 3.14 * i * i)
    
    print(vtup)

    # ---- Print an empty array
    print(np.array([]))
