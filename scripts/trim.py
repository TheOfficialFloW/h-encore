#!/usr/bin/env python2
from sys import argv, exit
import struct

def main():
  if len(argv) != 2:
    print("Usage: trim.py binary")
    return -1

  with open(argv[1], "rb") as f:
    data = f.read()

  new_len = len(data)
  if new_len < 4:
    return -1

  for i in range(new_len-4, 0, -4):
    word, = struct.unpack("<I", data[(i):(i+4)])
    if word != 0:
      new_len = i
      break

  open(argv[1], "wb").write(data[:new_len+4])

if __name__ == "__main__":
  exit(main())
