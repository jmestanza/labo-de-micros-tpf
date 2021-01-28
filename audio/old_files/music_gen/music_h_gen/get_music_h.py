from scipy.io import wavfile
import matplotlib.pyplot as plt 
import numpy as np
def get_string_from_16bit_value(val):
        i2s_first_byte = (val&0x00FF) 
        i2s_first_byte_str = "0x"+format(i2s_first_byte, '02x').upper()

        i2s_second_byte = (val&0xFF00) >> 8
        i2s_second_byte_str = "0x"+format(i2s_second_byte, '02x').upper()

        return i2s_first_byte_str+', '+i2s_second_byte_str+', '

#entra un archivo de 16 bit a 16khz
# samplerate, data = wavfile.read('sonic_16bit_16khz.wav')
file_name = 'sonic_16bit_16khz'

samplerate, data = wavfile.read("wav_files/"+file_name+".wav")

left_channel = data[:,0]
right_channel = data[:,1]

# plt.plot(left_channel)
# plt.show()

# val = left_channel[1]
# print(hex(val))
# val = left_channel[1]
# print(hex(val))


middle_string = ""

# print(type(left_channel[0]))

# original_size = len(right_channel)

# left_channel = np.hstack((np.zeros(original_size).astype(np.int16),left_channel))

# right_channel = np.hstack((np.zeros(original_size).astype(np.int16),right_channel))



# zero_padding = ["0x00,"]*len(right_channel)*4


# for i in range(len(zero_padding)):
#         middle_string += str(zero_padding[i]) 

for i in range(len(left_channel)):
        middle_string += get_string_from_16bit_value(left_channel[i])
        middle_string += get_string_from_16bit_value(right_channel[i])

middle_string = middle_string[:-2]
total_string = '['+middle_string+']'

want_to_write_array = False
if want_to_write_array:
        f = open(file_name+"_i2s_format", "w")
        f.write(total_string)
        f.close()

lines = middle_string.split(',') # los ultimos dos son " ,"

int_values = [int(el,16) for el in lines] # lista con todos los valores

BUFFER_SIZE = 1600
print(len(int_values))

N_BUFFERS = len(int_values)//BUFFER_SIZE
print("Entran exactamente ", N_BUFFERS , "BUFFERS")


f = open("template_music_h/music.h", "r")

line_list = []
idx_llave = -10000
idx_music_len = -10000
for i,x in enumerate(f):
        line_list.append(x)
        if "MUSIC_LEN" in x:
                idx_music_len = i
        if "{" in x:                
                idx_llave = i
f.close()

line_list[idx_music_len] = "#define MUSIC_LEN ("+str(N_BUFFERS*BUFFER_SIZE)+")\n"

# line_list.insert(idx_llave+1,"hola\n")

# middle_string = 
middle_string_modified = "\t\t"
for i in range(len(middle_string)):
        if ((i+1)%(21*5+3)) == 0:
                middle_string_modified += middle_string[i]+'\n\t\t'
        else:
                middle_string_modified += middle_string[i]


line_list.insert(idx_llave+1,middle_string_modified+'\n')

f = open("generated_music_h/generated_"+file_name+".h","w")

for i in range(len(line_list)):
        f.write(line_list[i])
f.close()

print("Se copio el arreglo a generated_music_h/generated_"+file_name+".h")