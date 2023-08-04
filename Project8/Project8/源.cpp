#include <arm_neon.h>

void AES_Encrypt(const uint8_t* key, const uint8_t* input, uint8_t* output) {
    uint8x16_t key_schedule[11];
    uint8x16_t state = vld1q_u8(input);

    // Key expansion
    key_schedule[0] = vld1q_u8(key);
    for (int i = 1; i < 11; i++) {
        key_schedule[i] = vaeseq_u8(key_schedule[i - 1], vdupq_n_u8(0));
        key_schedule[i] = vaesmcq_u8(key_schedule[i]);
        key_schedule[i] = veorq_u8(key_schedule[i], vld1q_u8(&key[i * 16]));
    }

    // Initial AddRoundKey
    state = veorq_u8(state, key_schedule[0]);

    // 9 rounds of AES encryption
    for (int round = 1; round <= 10; round++) {  // 将循环条件中的9改为10，使其执行10轮加密
        state = vaeseq_u8(state, vdupq_n_u8(0));
        state = vaesmcq_u8(state);
        state = veorq_u8(state, key_schedule[round]);
    }

    // Store the result in the output array
    vst1q_u8(output, state);
}
