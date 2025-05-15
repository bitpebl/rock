import siphash
# siphash-cffi package

# yes, bigrocks
key = b'bigrocks'

inputs = [
    "hey reader! pbl here (the person that made this)",
    "beep boop beep boop beep blip bloop glorp shroom" # idk what the fuck this is
]

# print constants
for s in inputs:
    print(f"// \"{s}\"")
    for i in range(8):
        data = f"{s}::{i}".encode("utf-8")
        h_bytes = siphash.siphash_64(key, data, 4, 8) # returns 8-byte result
        h_int = int.from_bytes(h_bytes, byteorder='little') # convert to int, big endian
        print(f"0x{h_int:016X}ULL,")
    print()
