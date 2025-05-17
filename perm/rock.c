// i... don't even know where to start
// okay so this is a permutation i made
// because i was super bored
// and then i put it through dieharder
// and got zero fails and one warning
// WITHOUT RESEEDING AND ONLY 64 BITS
// OF ENTROPY???

#include <stdint.h>
#include <stddef.h>

#define ROUNDS 16

static inline uint64_t rotl64(uint64_t x, unsigned r) {
    return (x << r) | (x >> (64 - r));
}

// mix 2 words a, b with rotation r
static inline void mix(uint64_t *a, uint64_t *b, unsigned r) {
    uint64_t A = *a, B = *b;
    A += B;
    B = rotl64(B, r) ^ A;
    *a = A;
    *b = B;
}

// rotation schedules
static const unsigned Rh[4]  = {13, 47, 30, 11};
static const unsigned Rh2[4] = {17, 29, 37, 43};
static const unsigned Rv[4]  = {32, 59, 22, 19};
static const unsigned Rv2[4] = {41, 05, 58, 07};

// derived with siphash-4-8 on the given strings, little-endian, key "therocks"
// see genconstants.py for info
static const uint64_t RC[ROUNDS] = {
    // "hey reader! pbl here (the person that made this)"
    0xB72EFDD79263528BULL,
    0x53A1596A6810951BULL,
    0xD95DED779506DB52ULL,
    0x07E4EC7DC8E8FCA7ULL,
    0xC2420E8E8C922005ULL,
    0x0206598042F5711FULL,
    0x12B663ED631F715EULL,
    0x75BBD7A3468DAD6FULL,

    // "beep boop beep boop beep blip bloop glorp shroom"
    0x38FC3A3A18564EF5ULL,
    0xE4A5895FAD1D94C2ULL,
    0x3F0E76A395FFFCA1ULL,
    0xC8CD3B0DB02FA3A1ULL,
    0xD9CB5F67A65637BDULL,
    0x670836315FC0EA48ULL,
    0xD6564F740A6F8899ULL,
    0x65107E2270075377ULL,
};

void rock(uint64_t S[8][8]) {
    for (size_t r = 0; r < ROUNDS; r++) {
        // inject rc
        S[0][0] += RC[r];

        // horizontal mixing
        for (int i = 0; i < 8; i++) {
            // even–odd pairs: (0,1), (2,3), (4,5), (6,7)
            for (int k = 0; k < 4; k++) {
                int j = 2*k;
                mix(&S[i][j], &S[i][j+1], Rh[k]);
            }
            // odd–even wrap: (1,2), (3,4), (5,6), (7,0)
            for (int k = 0; k < 4; k++) {
                int j = 2*k + 1;
                mix(&S[i][j], &S[i][(j+1) & 7], Rh2[k]);
            }
        }

        // vertical mixing
        for (int j = 0; j < 8; j++) {
            // even–odd pairs down the column
            for (int k = 0; k < 4; k++) {
                int i = 2*k;
                mix(&S[i][j], &S[i+1][j], Rv[k]);
            }
            // odd–even wrap: (1,2),(3,4),(5,6),(7,0)
            for (int k = 0; k < 4; k++) {
                int i = 2*k + 1;
                mix(&S[i][j], &S[(i+1) & 7][j], Rv2[k]);
            }
        }
    }
}
