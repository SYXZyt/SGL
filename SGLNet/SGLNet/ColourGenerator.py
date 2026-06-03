import sys

def GetParts(line):
	array = []
	spl = line.split("(")[1].split(",")
	spl[1] = spl[1][:-3].strip()
	array.append(spl[0])
	array.append(spl[1])
	return array

file = open(sys.argv[1])

for line in file:
	part = GetParts(line)
	print(f"public static Colour {part[0]} => new(0x{part[1]});");

file.close()