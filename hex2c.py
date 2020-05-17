#!/usr/bin/env python3

import re
import argparse

parser = argparse.ArgumentParser()
parser.add_argument("-x", "--hexfile", help="Intel hex file", required=True)
parser.add_argument("-p", "--presets", help="Preset numbers", nargs='+', type=int, default=[0, 1, 2, 3, 4, 5, 6, 7])
args = parser.parse_args()

print("Opening Intel HEX dump file `{}`".format(args.hexfile))

with open(args.hexfile, 'r') as hexfh:
    lines =  hexfh.readlines()

# decode intel hex format
# 1024 * 4 bytes
# 8 presets -> 512 bytes / 128 lines by presets
# :04 0000 00 80400011 2B
#  04 bytes
#  address 0000 
#  record type 00 (data)
#  data 80 40 00 11
#  checksum

pattern = re.compile(r"^:04\w{4}00(\w{2})(\w{2})(\w{2})(\w{2})\w{2}\n$")

data = [[] for i in range(8)]

for preset in range(8):
    for j in range(preset*128, (preset+1)*128):
        match = pattern.search(lines[j])
        if match:            
            data[preset].append("0x" + match.group(1))
            data[preset].append("0x" + match.group(2))
            data[preset].append("0x" + match.group(3))
            data[preset].append("0x" + match.group(4))
        else:
            print("Warning line not decoded:\n{}".format(lines[j]))


for preset in args.presets:
    print("Exporting preset {}\n".format(preset))
    print("const uint8_t preset{}[] PROGMEM = {{".format(preset))
    a = []
    for i in range(32):
        a.append(", ".join(data[preset][i*16:(i+1)*16]))
    print(",\n".join(a))
    print("};\n")





