#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "rock.h"

// 1024 bit rate
#define RATE_BYTES   128
// 8x8 grid of 64 bit words
#define STATE_WORDS  64

void rockhash(uint8_t *out, size_t outlen,
                     const uint8_t *in, size_t inlen) {
    uint64_t S[8][8] = {0}; // zero state

    // XOR the entire first block (row 0) with the tag constant (see genconstants.py)
    const uint64_t TAG = 0xAFE7E951411BA26EULL;
    S[0][0] ^= TAG;

    // absorb phase
    while (inlen >= RATE_BYTES) {
        for (size_t i = 0; i < RATE_BYTES / 8; ++i) {
            size_t x = i % 8, y = i / 8;
            uint64_t chunk;
            memcpy(&chunk, in + i * 8, 8);
            S[y][x] ^= chunk;
        }
        rock(S);
        in    += RATE_BYTES;
        inlen -= RATE_BYTES;
    }

    // padding (multirate pad10*1)
    uint8_t block[RATE_BYTES];
    memset(block, 0, RATE_BYTES);
    memcpy(block, in, inlen);
    block[inlen] = 0x80;          // start padding
    block[RATE_BYTES - 1] ^= 1;   // end padding bit

    for (size_t i = 0; i < RATE_BYTES / 8; ++i) {
        size_t x = i % 8, y = i / 8;
        uint64_t chunk;
        memcpy(&chunk, block + i * 8, 8);
        S[y][x] ^= chunk;
    }

    rock(S);

    // squeeze phase
    size_t outpos = 0;
    while (outpos < outlen) {
        for (size_t i = 0; i < RATE_BYTES / 8 && outpos < outlen; ++i) {
            size_t x = i % 8, y = i / 8;
            uint64_t word = S[y][x];
            size_t remaining = outlen - outpos;
            size_t n = remaining < 8 ? remaining : 8;
            memcpy(out + outpos, &word, n);
            outpos += n;
        }
        if (outpos < outlen)
            rock(S);
    }
}
