#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <immintrin.h>
#include "rock.h"

#define WORDS 64
#define STATE_BYTES (WORDS * sizeof(uint64_t))

static void seed_state(uint64_t S[8][8]) {
    uint64_t *flat = (uint64_t*)S;
    for (int i = 0; i < WORDS; i++) {
        unsigned long long *p = (unsigned long long*)&flat[i];
        while (!_rdseed64_step(p)) {
            // spin until it succeeds
        }
    }
}

int main(void) {
    uint64_t state[8][8];

    // seed the sponge ONCE with only 8 bytes of entropy
    seed_state(state);

    // repeatedly permute and spit out bytes
    while (1) {
        rock(state);
        if (write(STDOUT_FILENO, state, STATE_BYTES) != STATE_BYTES) {
            perror("write");
            return 1;
        }
    }
    return 0;
}
