import os
import re

currDir = os.path.dirname(os.path.abspath(__file__))

filePath = os.path.join(currDir, 'input.txt')

mulRegex = r"mul\((-?\d+),(-?\d+)\)"
toggleRegex = r"(do\(\)|don't\(\))"

try:
    with open(filePath, 'r') as file:
        fileData = file.read()
        
        mulEnabled = True
        sumP1 = 0
        sumP2 = 0
        
        matches = re.findall(f"{mulRegex}|{toggleRegex}", fileData)
        
        for match in matches:
            if match[0] and match[1]:
                x, y = int(match[0]), int(match[1])
                product = x * y
                
                sumP1 += product
                
                if mulEnabled:
                    sumP2 += product
            elif match[2] == "do()":
                mulEnabled = True
            elif match[2] == "don't()":
                mulEnabled = False
        
        print("Part 1 Sum:", sumP1)
        print("Part 2 Sum:", sumP2)

except FileNotFoundError:
    print("The file 'input.txt' was not found in the current directory.")
