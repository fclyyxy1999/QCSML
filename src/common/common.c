#include "common.h"

static const u8 hex_to_byte[] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0,
    10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    10, 11, 12, 13, 14, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};
static const char byte_to_hex[] = "0123456789ABCDEF";


void bytes_from_str(void *byte, const char* hex, const u32 nbytes) {
    u8 *byt = byte;
    for (u32 i = 0; i < nbytes; i ++)
        byt[i] = hex_to_byte[(u8)hex[i << 1]] << 4 | hex_to_byte[(u8)hex[(i << 1) + 1]];
}

void bytes_to_str(const void *byte, char* hex, const u32 nbytes) {
    const u8 *byt = byte;
    for (size_t i = 0; i < nbytes; i++) {
        hex[i * 2] = byte_to_hex[byt[i] >> 4];
        hex[i * 2 + 1] = byte_to_hex[byt[i] & 0x0F];
    }
    hex[nbytes * 2] = '\0';
}