/* * rand.h
*
 * This header file defines functions for generating random bytes and filling buffers with random data.
 * It is used in cryptographic applications to ensure security through randomness.
 */
#ifndef QCSML_RAND_H
#define QCSML_RAND_H

#include "common.h"
typedef int (*rand_func)(void *buf, u32 len);

/** Generates random bytes and fills a buffer with random data.
 * @param buf Pointer to the buffer where random bytes will be stored.
 * @param len Length of the buffer in bytes.
 * @return 0 on success, -1 on failure.
 */
int rand_gen(void *buf, u32 len);

/** Registers a user-defined random number generator function.
 * @param func Pointer to the user-defined random number generator function.
 * @return 0 on success, -1 on failure.
 */
int rand_reg(rand_func func);

/** Fills a buffer with pseudo-random data based on a hexadecimal string input.
 * @param in Pointer to the hexadecimal string.
 * @param len Length of the buffer to be filled in bytes.
 * @return 0 on success, -1 on failure.
 */
int rand_pad(const char *in, u32 len);

#endif //QCSML_RAND_H
