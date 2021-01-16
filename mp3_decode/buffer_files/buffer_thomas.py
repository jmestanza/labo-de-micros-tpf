
f = open('buffer_thomas.txt','r')
Lines = f.readlines()[1:] 
# all_lines = []

buffer = []

for line in Lines: 
    aux_line = line.split("\t")
    value = aux_line[3]
    buffer.append(value)
f.close()

aux_str = "uint16_t ret_buffer["+str(len(buffer))+"]={\n"

columnas = 10

for i,val in enumerate(buffer):
    aux_str += str(val) + ", "
    if(i-(columnas-1))%columnas == 0:
        aux_str+='\n'

aux_str += "\n};"

# print(aux_str)
f = open('buffer_thomas_ret.txt','w')
f.write(aux_str)
f.close()