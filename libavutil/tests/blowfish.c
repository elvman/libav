/*
 * This file is part of Libav.
 *
 * Libav is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Libav is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Libav; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libavutil/blowfish.h"

#define NUM_VARIABLE_KEY_TESTS 34

/* plaintext bytes -- left halves */
static const uint32_t plaintext_l[NUM_VARIABLE_KEY_TESTS] = {
    0x00000000, 0xFFFFFFFF, 0x10000000, 0x11111111, 0x11111111,
    0x01234567, 0x00000000, 0x01234567, 0x01A1D6D0, 0x5CD54CA8,
    0x0248D438, 0x51454B58, 0x42FD4430, 0x059B5E08, 0x0756D8E0,
    0x762514B8, 0x3BDD1190, 0x26955F68, 0x164D5E40, 0x6B056E18,
    0x004BD6EF, 0x480D3900, 0x437540C8, 0x072D43A0, 0x02FE5577,
    0x1D9D5C50, 0x30553228, 0x01234567, 0x01234567, 0x01234567,
    0xFFFFFFFF, 0x00000000, 0x00000000, 0xFFFFFFFF
};

/* plaintext bytes -- right halves */
static const uint32_t plaintext_r[NUM_VARIABLE_KEY_TESTS] = {
    0x00000000, 0xFFFFFFFF, 0x00000001, 0x11111111, 0x11111111,
    0x89ABCDEF, 0x00000000, 0x89ABCDEF, 0x39776742, 0x3DEF57DA,
    0x06F67172, 0x2DDF440A, 0x59577FA2, 0x51CF143A, 0x774761D2,
    0x29BF486A, 0x49372802, 0x35AF609A, 0x4F275232, 0x759F5CCA,
    0x09176062, 0x6EE762F2, 0x698F3CFA, 0x77075292, 0x8117F12A,
    0x18F728C2, 0x6D6F295A, 0x89ABCDEF, 0x89ABCDEF, 0x89ABCDEF,
    0xFFFFFFFF, 0x00000000, 0x00000000, 0xFFFFFFFF
};

/* key bytes for variable key tests */
static const uint8_t variable_key[NUM_VARIABLE_KEY_TESTS][8] = {
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
    { 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 },
    { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF },
    { 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11 },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10 },
    { 0x7C, 0xA1, 0x10, 0x45, 0x4A, 0x1A, 0x6E, 0x57 },
    { 0x01, 0x31, 0xD9, 0x61, 0x9D, 0xC1, 0x37, 0x6E },
    { 0x07, 0xA1, 0x13, 0x3E, 0x4A, 0x0B, 0x26, 0x86 },
    { 0x38, 0x49, 0x67, 0x4C, 0x26, 0x02, 0x31, 0x9E },
    { 0x04, 0xB9, 0x15, 0xBA, 0x43, 0xFE, 0xB5, 0xB6 },
    { 0x01, 0x13, 0xB9, 0x70, 0xFD, 0x34, 0xF2, 0xCE },
    { 0x01, 0x70, 0xF1, 0x75, 0x46, 0x8F, 0xB5, 0xE6 },
    { 0x43, 0x29, 0x7F, 0xAD, 0x38, 0xE3, 0x73, 0xFE },
    { 0x07, 0xA7, 0x13, 0x70, 0x45, 0xDA, 0x2A, 0x16 },
    { 0x04, 0x68, 0x91, 0x04, 0xC2, 0xFD, 0x3B, 0x2F },
    { 0x37, 0xD0, 0x6B, 0xB5, 0x16, 0xCB, 0x75, 0x46 },
    { 0x1F, 0x08, 0x26, 0x0D, 0x1A, 0xC2, 0x46, 0x5E },
    { 0x58, 0x40, 0x23, 0x64, 0x1A, 0xBA, 0x61, 0x76 },
    { 0x02, 0x58, 0x16, 0x16, 0x46, 0x29, 0xB0, 0x07 },
    { 0x49, 0x79, 0x3E, 0xBC, 0x79, 0xB3, 0x25, 0x8F },
    { 0x4F, 0xB0, 0x5E, 0x15, 0x15, 0xAB, 0x73, 0xA7 },
    { 0x49, 0xE9, 0x5D, 0x6D, 0x4C, 0xA2, 0x29, 0xBF },
    { 0x01, 0x83, 0x10, 0xDC, 0x40, 0x9B, 0x26, 0xD6 },
    { 0x1C, 0x58, 0x7F, 0x1C, 0x13, 0x92, 0x4F, 0xEF },
    { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 },
    { 0x1F, 0x1F, 0x1F, 0x1F, 0x0E, 0x0E, 0x0E, 0x0E },
    { 0xE0, 0xFE, 0xE0, 0xFE, 0xF1, 0xFE, 0xF1, 0xFE },
    { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
    { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF },
    { 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF },
    { 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10 }
};

/* ciphertext bytes -- left halves */
static const uint32_t ciphertext_l[NUM_VARIABLE_KEY_TESTS] = {
    0x4EF99745, 0x51866FD5, 0x7D856F9A, 0x2466DD87, 0x61F9C380,
    0x7D0CC630, 0x4EF99745, 0x0ACEAB0F, 0x59C68245, 0xB1B8CC0B,
    0x1730E577, 0xA25E7856, 0x353882B1, 0x48F4D088, 0x432193B7,
    0x13F04154, 0x2EEDDA93, 0xD887E039, 0x5F99D04F, 0x4A057A3B,
    0x452031C1, 0x7555AE39, 0x53C55F9C, 0x7A8E7BFA, 0xCF9C5D7A,
    0xD1ABB290, 0x55CB3774, 0xFA34EC48, 0xA7907951, 0xC39E072D,
    0x014933E0, 0xF21E9A77, 0x24594688, 0x6B5C5A9C
};

/* ciphertext bytes -- right halves */
static const uint32_t ciphertext_r[NUM_VARIABLE_KEY_TESTS] = {
    0x6198DD78, 0xB85ECB8A, 0x613063F2, 0x8B963C9D, 0x2281B096,
    0xAFDA1EC7, 0x6198DD78, 0xC6A0A28D, 0xEB05282B, 0x250F09A0,
    0x8BEA1DA4, 0xCF2651EB, 0x09CE8F1A, 0x4C379918, 0x8951FC98,
    0xD69D1AE5, 0xFFD39C79, 0x3C2DA6E3, 0x5B163969, 0x24D3977B,
    0xE4FADA8E, 0xF59B87BD, 0xB49FC019, 0x937E89A3, 0x4986ADB5,
    0x658BC778, 0xD13EF201, 0x47B268B2, 0x08EA3CAE, 0x9FAC631D,
    0xCDAFF6E4, 0xB71C49BC, 0x5754369A, 0x5D9E0A5A
};

/* plaintext bytes */
static const uint8_t plaintext[8] = "BLOWFISH";

static const uint8_t plaintext2[16] = "BLOWFISHBLOWFISH";

/* ciphertext bytes */
static const uint8_t ciphertext[8] = {
    0x32, 0x4E, 0xD0, 0xFE, 0xF4, 0x13, 0xA2, 0x03
};

static const uint8_t ciphertext2[16] = {
    0x53, 0x00, 0x40, 0x06, 0x63, 0xf2, 0x1d, 0x99,
    0x3b, 0x9b, 0x27, 0x64, 0x46, 0xfd, 0x20, 0xc1,
};

#define IV "blowfish"

static void test_blowfish(AVBlowfish *ctx, uint8_t *dst, const uint8_t *src,
                          const uint8_t *ref, int len, uint8_t *iv, int dir,
                          const char *test)
{
    av_blowfish_crypt(ctx, dst, src, len, iv, dir);
    if (memcmp(dst, ref, 8*len)) {
        int i;
        printf("%s failed\ngot      ", test);
        for (i = 0; i < 8*len; i++)
            printf("%02x ", dst[i]);
        printf("\nexpected ");
        for (i = 0; i < 8*len; i++)
            printf("%02x ", ref[i]);
        printf("\n");
        exit(1);
    }
}

int main(void)
{
    AVBlowfish ctx;
    uint32_t tmptext_l[NUM_VARIABLE_KEY_TESTS];
    uint32_t tmptext_r[NUM_VARIABLE_KEY_TESTS];
    uint8_t tmp[16], iv[8];
    int i;

    av_blowfish_init(&ctx, "abcdefghijklmnopqrstuvwxyz", 26);

    test_blowfish(&ctx, tmp, plaintext, ciphertext, 1, NULL, 0, "encryption");
    test_blowfish(&ctx, tmp, ciphertext, plaintext, 1, NULL, 1, "decryption");
    test_blowfish(&ctx, tmp, tmp, ciphertext, 1, NULL, 0, "Inplace encryption");
    test_blowfish(&ctx, tmp, tmp, plaintext,  1, NULL, 1, "Inplace decryption");
    memcpy(iv, IV, 8);
    test_blowfish(&ctx, tmp, plaintext2, ciphertext2, 2, iv, 0, "CBC encryption");
    memcpy(iv, IV, 8);
    test_blowfish(&ctx, tmp, ciphertext2, plaintext2, 2, iv, 1, "CBC decryption");
    memcpy(iv, IV, 8);
    test_blowfish(&ctx, tmp, tmp, ciphertext2, 2, iv, 0, "Inplace CBC encryption");
    memcpy(iv, IV, 8);
    test_blowfish(&ctx, tmp, tmp, plaintext2,  2, iv, 1, "Inplace CBC decryption");

    memcpy(tmptext_l, plaintext_l, sizeof(*plaintext_l) * NUM_VARIABLE_KEY_TESTS);
    memcpy(tmptext_r, plaintext_r, sizeof(*plaintext_r) * NUM_VARIABLE_KEY_TESTS);

    for (i = 0; i < NUM_VARIABLE_KEY_TESTS; i++) {
        av_blowfish_init(&ctx, variable_key[i], 8);

        av_blowfish_crypt_ecb(&ctx, &tmptext_l[i], &tmptext_r[i], 0);
        if (tmptext_l[i] != ciphertext_l[i] || tmptext_r[i] != ciphertext_r[i]) {
            printf("Test encryption failed.\n");
            return 2;
        }

        av_blowfish_crypt_ecb(&ctx, &tmptext_l[i], &tmptext_r[i], 1);
        if (tmptext_l[i] != plaintext_l[i] || tmptext_r[i] != plaintext_r[i]) {
            printf("Test decryption failed.\n");
            return 3;
        }
    }
    printf("Test encryption/decryption success.\n");

    return 0;
}

