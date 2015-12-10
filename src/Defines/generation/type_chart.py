import re
with open('C:/Users/Dilshan/Desktop/Battle-System/src/Defines/generation/type_chart') as f:
     lines = f.read().split("_")
     
lines[:] = (element for element in lines if element != "")
with open('C:/Users/Dilshan/Desktop/Battle-System/src/Defines/type_chart.txt', "w") as f:
     for line in lines:
          line = line.replace("\n\t", "").replace(":", "").upper().split(" ")[1:len(line.split(" ")) -1]
          temp = list(line)
          for element in line:
               if (element.isalpha()):
                    temp.remove(element)
          f.write(str(temp[12]) + str(temp[5]) + str(temp[7]) + str(temp[13]) + str(temp[10]) + 
          str(temp[15]) + str(temp[0]) + str(temp[8]) + str(temp[16]) + str(temp[18]) + str(temp[6]) +
          str(temp[17]) + str(temp[9]) + str(temp[3]) + str(temp[14]) + str(temp[11]) + 
          str(temp[2]) + str(temp[1]) + str(temp[4]))
          

