import numpy as np 
from scipy.io.wavfile import write

samplerate = 16000; fs = 1000
t = np.linspace(0., 1., samplerate)
amplitude = np.iinfo(np.int16).max  # 16 bit!
data = amplitude * np.sin(2. * np.pi * fs * t)
filename = "example.wav" 
write(filename, samplerate, data.astype(np.int16))
