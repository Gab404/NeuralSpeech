import numpy as np

def mel(f):
    return 2595 * np.log10(1 + f / 700)

def inv_mel(m):
    return 700 * (10**(m / 2595) - 1)

def mel_filterbank(n_filters, n_fft, fs, fmin=0, fmax=None):
    if fmax is None:
        fmax = fs / 2

    mel_points = np.linspace(mel(fmin), mel(fmax), n_filters + 2)
    hz_points = inv_mel(mel_points)
    bin_points = np.floor((n_fft + 1) * hz_points / fs).astype(int)

    filters = np.zeros((n_filters, n_fft // 2))
    for i in range(1, n_filters + 1):
        left, center, right = bin_points[i-1:i+2]
        for j in range(left, center):
            filters[i-1, j] = (j - left) / (center - left)
        for j in range(center, right):
            filters[i-1, j] = (right - j) / (right - center)
    return filters

filters = mel_filterbank(n_filters=20, n_fft=256, fs=8000)
with open('./src/mfcc.cpp', 'w', newline='\n', encoding='utf-8') as f:
    f.write(f"const float mel_filters[{len(filters)}][{len(filters[0])}] PROGMEM = {{\n")
    for i, filt in enumerate(filters):
        line = ", ".join(f"{x:.6f}f" for x in filt)
        f.write(f"    {{ {line} }}")
        if i < len(filters) - 1:
            f.write(",")
        f.write("\n")
    f.write("};")