<div id="top"></div>

[![LinkedIn](https://img.shields.io/badge/LinkedIn-Connect-blue?style=for-the-badge&logo=linkedin)](https://www.linkedin.com/in/gabriel-guiet-dupre/)

<br />

<p align="center">
  <strong><font size="6">Neural Speech</font></strong>
</p>

<p align="center">
  <img src="./assets/arduino-due.png" alt="arduino-due" width="20%" />
</p>

<p align="center">
  Voice recognition on Arduino Due – ECE Paris Engineering Project
</p>

---

## Audio Processing Pipeline

The audio signal undergoes the following processing steps:

1. **Sampling & Filtering**  
   - Sampled at **32 kHz**  
   - FIR Band-pass filter: **300–3400 Hz**  
   - Stop bands: **0–300 Hz** and **4000–16000 Hz**

<p align="center">
  <img src="./assets/RIF used.png" alt="RIF Used" width="50%" />
</p>
<p align="center"><strong>RIF Used</strong></p>

2. **Downsampling**  
   - Reduced to **8 kHz** for lightweight processing on embedded device

3. **Framing & Overlap**  
   - Frame size: **256 samples**  
   - Overlap: **37.5%** (96 samples)

4. **Preprocessing**  
   - **AGC (Automatic Gain Control)**  
   - **FFT** to convert signal to frequency domain

5. **Mel Filterbank & MFCC Extraction**  
   - Apply **Mel filters**  
   - Logarithmic compression  
   - Discrete Cosine Transform (DCT)  
   - Extract **12 MFCCs per frame**

<p align="center">
  <img src="./assets/Mel_Filter_Application.png" alt="Mel Filter Application" width="45%" />
  <img src="./assets/DCT_Matrix_Application.png" alt="DCT Matrix Application" width="50%" />
</p>
<p align="center"><strong>Mel Filter Application (left) & DCT Matrix Application (right)</strong></p>

6. **Temporal Context**  
   - Process repeated for **50 frames** = **1 second of audio**  
   - Final MFCC matrix shape: **[50 × 12]**

---

## Deep Learning for Classification

After computing the MFCC matrix, it is passed into a lightweight CNN for binary classification.

### Model Architecture

- **Input:** `(50, 12, 1)` MFCC matrix  
- **Conv2D (8 filters, 3×3)** + **ReLU**  
- **MaxPooling2D (2×2)**  
- **Conv2D (8 filters, 3×3)** + **ReLU**  
- **MaxPooling2D (2×2)**  
- **Flatten**  
- **Dense (8 units, ReLU)**  
- **Dense (1 unit, Sigmoid)** → Binary output

<p align="center">
  <img src="./assets/model.png" alt="CNN Model" width="100%" />
</p>
<p align="center"><strong>My CNN Model</strong></p>

The CNN detects local patterns in the MFCC matrix and classifies 1s audio segments.  
The processing is **real-time**: ~1.5s for recording, processing, and prediction.

---

## Deep Learning for Classification

One second after clicked the button, the green LED turns on. While It's on It means you can speak. You have one second to speak. 

Please note that the dataset has been made with just one voice so can't work with other voices. You must update the dataset and train the model again on the new one.

### Hardware Architecture

- **Arduino Due**  
- **Microphone MAX9814**  (A0)
- **LEDs** (Yellow D5, White D3, Green D2)  
- **Push Button**  (D4)

<p align="center">
  <img src="./assets/shema_hardware.png" alt="hardware" width="100%" />
</p>
<p align="center"><strong>Hardware Shema</strong></p>


---

## Built With

- [PlatformIO](https://platformio.org/)
- [Arduino Due](https://docs.arduino.cc/hardware/due/)
- [Python 3.7](https://www.python.org/downloads/release/python-370/)
- [Tensorflow 2.2](https://www.tensorflow.org/install?hl=fr)
- [EloquentTinyML](https://github.com/eloquentarduino/EloquentTinyML)
- [ArduinoFFT](https://github.com/kosme/arduinoFFT)

---

## 📫 Contact

Gabriel GUIET-DUPRE  
[![LinkedIn](https://img.shields.io/badge/LinkedIn-Connect-blue?style=flat-square&logo=linkedin)](https://www.linkedin.com/in/gabriel-guiet-dupre/)  
📧 gabriel.guietdupre@edu.ece.fr

[🔗 Project Link](https://github.com/Gab404/NeuralSpeech)
