import glob

txtfiles = []
for file in glob.glob("*.txt"):
    txtfiles.append(file)

index_file = 0

for txt_file in txtfiles:
    f = open(txt_file,'r')
    lines = []
    for x in f:    
        lines.append(x.replace('\n',''))

    new_order = []

    for line in lines:
        line_arr = line.split(" ")
        for i in range(0,len(line_arr),4):
            new_order.append(line_arr[i+3]+line_arr[i+2]+line_arr[i+1]+line_arr[i])

    # print(new_order)

    final_str = ""
    Address = 0
    for i in range(len(new_order)):
        final_str += "Address = " + hex(Address) + " Value = " + new_order[i] + "\n"
        Address += 4
    f.close()

    f = open("better_"+txt_file,"w")
    f.write(final_str)
    f.close()
