#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <immintrin.h>
#include "rock.h"
#include "unif01.h"
#include "bbattery.h"

#define WORDS 64
#define STATE_BYTES (WORDS * sizeof(uint64_t))
#define ENTROPY_BYTES 32

static uint64_t state[8][8];
static int state_index = WORDS; // start full so we trigger initial perm

static void seed_state(uint64_t S[8][8]) {
    uint64_t *flat = (uint64_t *)S;
    memset(flat, 0, STATE_BYTES);

    unsigned long long entropy;
    while (!_rdseed64_step(&entropy)) {
        // spin until rdseed succeeds
    }

    flat[0] = entropy;
}

// oh how i hate pipe latency
static unsigned int rock_rng(void) {
    static uint64_t *flat = (uint64_t *)state;

    if (state_index >= WORDS) {
        rock(state);         // permute
        state_index = 0;     // reset index
    }

    uint64_t val = flat[state_index++];
    // chop 64-bit word into two 32-bit words on alternate calls
    static int toggle = 0;
    toggle ^= 1;
    return toggle ? (uint32_t)(val >> 32) : (uint32_t)(val & 0xFFFFFFFF);
}

int main(void) {
    printf("seeding with 32 bytes of entropy from rdseed");
    
    seed_state(state);

    unif01_Gen *gen = unif01_CreateExternGenBits("rock_rng", rock_rng);

    bbattery_BigCrush(gen);

    unif01_DeleteExternGenBits(gen);
    return 0;
}

