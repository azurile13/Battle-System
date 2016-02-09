import re

def capitalize(str):
    if len(str):
        return (str[0].upper() + str[1:])
    return ""

def lowercase(str):
    if len(str):
        return (str[0].lower() + str[1:]) 
    return ""


with open('C:/Users/Dilshan/Documents/Hack/Table.txt') as f:
    lines = f.read().splitlines()
for i in range (0, len(lines)):
    lines[i] = lines[i].split('=')
    
    
with open('C:/Users/Dilshan/Desktop/Battle-system/src/defines/generation/battle_strings') as f:
    to_convert = f.read().splitlines()

    
with open('C:/Users/Dilshan/Desktop/Battle-system/src/strings.h', "w") as f:
    f.write("#ifndef STRINGS_STRINGS_H_" + "\n" + "#define STRINGS_STRINGS_H_" +
            ("\n" * 2))
    result = ""
    flag = 0
    temp = ""
    for convert in to_convert:
        f.write("char ca_" + lowercase(convert.split("=")[0]) + "[] = { ")
        for char in (convert.split("=")[1]):
            if (char == "|"):
                if (flag):
                    if (len(temp) == 2):
                        result += "0x" + temp + ", "
                    else:
                        result += ("0x" + temp[:2] + ", 0x" + temp[2:] + ", ")
                    temp = ""
                flag = not(flag)
            if (flag and char != "|"):
                temp += char
            else:
                for element in lines:
                    if element[1] == char:
                        result += ("0x" + (element[0]) + ", ")

        f.write(result[:len(result) -2] + ", 0xFF };" + ("\n" * 2))
        result = ""
    f.write("char ca_Mega[] = { 0xC7, 0xD9, 0xDB, 0xD5, 0xFF };" + ("\n" *2))
    f.write("char ca_Primal[] = { 0xCA, 0xE6, 0xDD, 0xE1, 0xD5, 0xE0, 0xFF };" +
            "\n" *2)
    f.write("\n"  + "#endif /* STRINGS_STRINGS_H */" + ("\n"))

