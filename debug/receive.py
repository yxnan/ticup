#!/usr/bin/python3

import sys, os
import serial

retval = 0

if len(sys.argv) >= 2:
    port = sys.argv[1]
else:
    port = '/dev/ttyUSB0'

s = serial.Serial(port, 115200, timeout=3)

prefix = 'data_receive/'
index_file = open(prefix + 'index', 'r+')
index = int(index_file.readline().strip())
filename = str(index + 1)

with open(prefix + filename, 'w') as f:
    line = s.readline().decode('utf-8')
    if line == '':
        retval = -1
    else:
        f.writelines(line)
        index_file.seek(0)
        index_file.writelines(filename)
        print(prefix + filename, end='')
        # os.system('./plot ' + line)

index_file.close()
sys.exit(retval)
