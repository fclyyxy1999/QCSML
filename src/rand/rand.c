#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include "rand.h"

// The mode of random number generation,
enum {
    TRNG,       // True Random Number Generator
    FRNG,       // Fake Random Number Generator
    URNG,       // User-defined Random Number Generator
} mode = TRNG;

// pseudo-random state, used only in PRNG mode
struct {
    u8 *buf;
    u32 len;
    u32 pos;
} pst = {NULL, 0, 0};

// User-defined Random Number Generator
rand_func urng = NULL;

// True Random Number Generator
static int trng(u8 *buf, const u32 len) {
#ifdef __linux__
    const int fd = open("/dev/urandom", O_RDONLY);
    if (fd < 0) {
        perror("unable to open /dev/urandom");
        return -1;
    }

    ssize_t bytes_read = read(fd, buf, len);
    close(fd);

    if (bytes_read < 0 || (u32)bytes_read != len) {
        perror("unable to read from /dev/urandom");
        return -1;
    }
    return 0;
#else
    // 非Linux平台的备选实现
    // fprintf(stderr, "True random mode not supported on this platform\n");
    memset(buf, 0, len); // Fill with zeros as a fallback
    return -1;
#endif
}

// Fake Random Number Generator
static int frng(u8 *buf, const u32 len) {
    if (pst.buf == NULL)
        return -1;

    // check if the requested length exceeds the available pseudo-random data
    const u32 available = pst.len - pst.pos;

    if (len <= available) {
        // enough pseudo-random data available
        memcpy(buf, pst.buf + pst.pos, len);
        pst.pos += len;
        return 0;
    } else {
        // not enough pseudo-random data
        memcpy(buf, pst.buf + pst.pos, available);
        free(pst.buf);
        pst.buf = NULL;
        pst.len = 0;
        pst.pos = 0;
        mode = urng == NULL ? TRNG : URNG;
        return rand_gen(buf + available, len - available);
    }
}

int rand_reg(const rand_func func) {
    if (func == NULL)
        return -1;
    mode = URNG;
    urng = func;
    return 0;
}

int rand_gen(void *buf, const u32 len) {
    if (buf == NULL)
        return -1;
    if (len == 0)
        return 0;
    switch (mode) {
        case TRNG:
            return trng(buf, len);
        case FRNG:
            return frng(buf, len);
        case URNG:
            return urng(buf, len);
        default:
            return -1;
    }
}

int rand_pad(const char *in, const u32 len) {
    if (in == NULL)
        return -1;

    // clean buf if it exists
    if (pst.buf)
        free(pst.buf);

    u8 *buf = malloc(len);
    if (buf == NULL) {
        perror("malloc failed");
        return -1;
    }

    bytes_from_hexstr(buf, in, len);

    mode = FRNG;
    pst.buf = buf;
    pst.len = len;
    pst.pos = 0;

    return 0;
}