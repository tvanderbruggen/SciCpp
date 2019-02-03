# SPDX-License-Identifier: MIT
# Copyright (c) 2019 Thomas Vanderbruggen <th.vanderbruggen@gmail.com>

import numpy as np
from scipy.signal import flattop

if __name__ == "__main__":
    noise = np.random.randn(10000)
    print("Noise: mean = {}, std = {}".format(np.mean(noise), np.std(noise)))
    w = flattop(len(noise))
    W = np.sum(w ** 2)
    fs = 1.0
    psd = 2.0 * np.absolute(np.fft.rfft(noise * w)) ** 2 / (fs * W)
    # The expected PSD for a white noise is 2 * sigma ^ 2
    print("PSD = {}".format(np.mean(psd)))
