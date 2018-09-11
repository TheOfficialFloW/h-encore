#!/usr/bin/env python2
from sys import argv, exit
import random
import struct

gadgets = [
  0x8102a2f3,
  0x8102a3d3,
  0x81005579,
  0x8100a107,
  0x8100a16b,
  0x8100717b,
  0x8102978d,
  0x81000c69,
  0x810321ed,
  0x810321fd,
  0x81021795,
  0x81000065,
  0x8102675d,
  0x81000bf5,
  0x81026bad,
]

def main():
  if len(argv) != 3:
    print("Usage: obfuscate.py binary stage_number")
    return -1

  if argv[2] == "1":
    random.seed(42)
  else:
    random.seed(69)

  with open(argv[1], "rb") as f:
    data = bytearray(f.read())

  for i in range(0, len(data), 4):
    word, = struct.unpack("<I", data[(i):(i+4)])
    if word == 0xDEADBEEF:
      random_gadget = struct.pack("I", word)
      valid = False
      while not valid:
        random_choice = random.randint(0, 10)
        if random_choice == 0:
          random_number = random.randrange(0x810c0d00, 0x810c1000, 0x04)
        elif random_choice == 1:
          random_number = random.randrange(0x81e82000, 0x81ea2000, 0x04)
        elif random_choice >= 2 and random_choice < 4:
          random_number = random.randrange(0x8102f294, 0x81030044, 0x10)
        elif random_choice >= 4 and random_choice < 6:
          random_number = random.randint(0x81000000, 0x8102f294) | 0x1
        else:
          random_number = random.choice(gadgets)
        random_gadget = struct.pack("I", random_number)
        valid = True
        if argv[2] == "1" and random_number == 0xffff:
          valid = False
      data[(i):(i+4)] = random_gadget

  open(argv[1], "wb").write(data)

if __name__ == "__main__":
  exit(main())
