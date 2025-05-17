# build time: 0.011
# newline cleanup time: 0.0004
# autopep8 time: 0.0858
# Python3.13 for CPython compiled by ASnake v0.13.42
# tysm ahri for https://asnake.org!!!

import siphash  # from the siphash-cffi package

# round constant things
inputs = ["hey reader! pbl here (the person that made this)", "beep boop beep boop beep blip bloop glorp shroom"] # idk i was tired and this sounded funny

# apparently it boosts perf if you cache print??
ASprint = print

for s in inputs:
    ASprint(f"// \"{s}\"")
    for i in range(8):
        data = f"{s}::{i}".encode("utf-8")
                                     # yes, bigrocks 🪨🔥
        h_bytes = siphash.siphash_64(b'bigrocks', data, 4, 8) # eg. siphash-4-8("hey reader! pbl here (the person that made this)::5")
        h_int = int.from_bytes(h_bytes, byteorder='little') # little endian
        ASprint(f"0x{h_int:016X}ULL,")

    ASprint()

sponge_bytes = siphash.siphash_64(b'bigrocks', b"rock sponge", 4, 8)

# generate sponge tag for "rock sponge"
sponge_tag = int.from_bytes(sponge_bytes, byteorder='little')
print("// sponge tag for \"rock sponge\"")
print(f"0x{sponge_tag:016X}ULL")
