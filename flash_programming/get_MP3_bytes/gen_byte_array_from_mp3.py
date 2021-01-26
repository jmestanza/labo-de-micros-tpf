from pydub import AudioSegment
from pydub.playback import play
import io
import os
import glob

print(os.listdir())

all_mp3 = []
os.chdir("mp3_files_8KHz")
for file in glob.glob("*.mp3"):
    all_mp3.append(file)

os.chdir('..')

SECTORS_NEEDED = {}
TOTAL_SECTORS_NEEDED = 0
SECTOR_INDEX = {}
for filename in all_mp3:
    data = open("mp3_files_8KHz/"+filename, 'rb').read()
    curr_bytes = len(data)
    NUM_SECTORS = curr_bytes//(2*1024) + 1
    # print(filename+" va a requerir: " , NUM_SECTORS)
    SECTORS_NEEDED[filename] = NUM_SECTORS
    TOTAL_SECTORS_NEEDED += NUM_SECTORS
    SECTOR_INDEX[filename] = TOTAL_SECTORS_NEEDED

print(f"numero de sectores utilizados: {TOTAL_SECTORS_NEEDED} lo cual es  { (TOTAL_SECTORS_NEEDED*4096*2)} Bytes")
print(f"Flash interna % ocupada por mp3 =   { (TOTAL_SECTORS_NEEDED*4096*2) *100/ (1024*1000)}  %")

for filename in all_mp3:
    print(SECTORS_NEEDED[filename])
    print(SECTOR_INDEX[filename])

# filename = "ecg_oor"
    data = open("mp3_files_8KHz/"+filename, 'rb').read()

    # hex_arr = [el for el in data]

    # print(hex_arr[:10])


    print(f"El archivo {filename} tiene {len(data)} bytes")

    arr_str = "uint8_t "+filename[:-4]+"_array[BUFFER_LEN]={ \n"

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

            
    f = open("generated_byte_array/"+filename[:-4]+"_music_array.h",'w')
    f.write("#define "+filename[:-4].upper()+"\n")
    f.write("#define SECTOR_INDEX_FROM_END "+str(SECTOR_INDEX[filename])+"\n")
    f.write("#define BUFFER_LEN "+str(len(data))+"\n")

    f.write(arr_str)

    f.close()


# el mp3 de ecg esta en   => 0xf0000
# el mp3 del spo2 esta en => 0xe0000