from pydub import AudioSegment
from pydub.playback import play
import io

filename = "thomas_16k"
data = open(filename+'.mp3', 'rb').read()

# hex_arr = [el for el in data]

# print(hex_arr[:10])


print(f"El archivo {filename} tiene {len(data)} bytes")

arr_str = "uint8_t "+filename+"_array[BUFFER_LEN]={ \n"

# arr_str = ""

columns = 30

for i,el in enumerate(data):
    hex_num = "0x"+format(el, '02x')
    arr_str += hex_num + ","
    if((i-(columns-1)) % columns == 0):
        arr_str += "\n"

if(arr_str[-1] == ","):
    arr_str = arr_str[:-1]
arr_str += "};"

        
f = open(filename+"_bytes.txt",'w')
f.write("#define BUFFER_LEN "+str(len(data))+"\n")
f.write(arr_str)

f.close()
