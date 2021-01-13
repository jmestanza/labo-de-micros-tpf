import numpy as np
from pydub import AudioSegment
AudioSegment.converter = "ffmpeg.exe"
AudioSegment.ffmpeg = "ffmpeg.exe"
AudioSegment.ffprobe = "ffprobe.exe"

def read(f, normalized=False):
    """MP3 to numpy array"""
    a = AudioSegment.from_mp3(f)
    y = np.array(a.get_array_of_samples())
    if a.channels == 2:
        y = y.reshape((-1, 2))
    if normalized:
        return a.frame_rate, np.float32(y) / 2**15
    else:
        return a.frame_rate, y


def array_to_txt(array, output_name):
    np.savetxt(output_name, array, delimiter=',')
    file1 = open(output_name, 'r')
    Lines = file1.readlines()
    output = "{"
    count = 0
    for line in Lines:
        output += line.strip() + ","
        if count % 4 == 0:
            output += "\n"
        count += 1
    print(count)
    output = output[:-1]
    output += "}"

    text_file = open(output_name, "w")
    n = text_file.write(output)

fr, data =read('sonic.mp3')

L = data[:, 0]
R = data[:, 1]

array_to_txt(L, "LeftChannel.txt")
array_to_txt(R, "RightChannel.txt")