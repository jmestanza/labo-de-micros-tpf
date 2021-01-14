from pydub import AudioSegment
from pydub.playback import play
import io

filename = "sonic"
data = open(filename+'.mp3', 'rb').read()

# hex_arr = [el for el in data]

# print(hex_arr[:10])

arr_str = "uint8_t "+filename+"_array[]={ \n"

# arr_str = ""

for i,el in enumerate(data):
    hex_num = "0x"+format(el, '02x')
    arr_str += hex_num + ","
    if((i-9) % 10 == 0):
        arr_str += "\n"

if(arr_str[-1] == ","):
    arr_str = arr_str[:-1]
arr_str += "}"

        
f = open(filename+"_bytes.txt",'w')

f.write(arr_str)

f.close()
