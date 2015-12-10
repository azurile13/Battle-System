import re
with open('C:/Users/Dilshan/Desktop/Battle-System/src/Defines/generation/abilities') as f:
     lines = f.read().splitlines()

with open('C:/Users/Dilshan/Desktop/Battle-System/src/Defines/abilities.h', "w") as f:
     # header guards     
     f.write("#ifndef ABILITY_IDS" + "\n" + "#define ABILITY_IDS" + "\n"*3)  
     for line in lines:
          hex_val = hex(int(line.split("\t")[0]))
          new_str = ""
          for char in hex_val:
               if (char != "x"):
                    new_str += char.capitalize()
               else:
                    new_str += char
          f.write("#define " + (line.split("\t")[1]).upper() + " " + new_str + "\n")

     #header guards end
     f.write("\n" + "#endif /* ABILITY_IDS */" + "\n")