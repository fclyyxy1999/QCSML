/**
* @file sm2.h
 * @brief This file contains declarations of structures and functions related to SM3 algorithm.
 * @date 2025-01-07
 * @author RunChen Zhao
 * @email zrc1024@foxmail.com
 * @version 2.0
 */

#ifndef SM_SM3_H
#define SM_SM3_H

#include "common.h"


/// SM3 digest type.
typedef u8 sm3_digest[32];

/// SM3 hash context structure.
typedef struct SM3Context{
    u32 reg[8];     ///< Array of 8 32-bit registers used in the SM3 algorithm.
    u8  msg[64];    ///< Array of 64 bytes to hold the message being processed.
    u64 len;        ///< 64-bit integer representing the length of the message in bytes.
} sm3_ctx;

/**
 * @brief Compresses the message blocks using the SM3 compression function.
 * @param[out] buf The current hash state.
 * @param[in] msg A pointer to the message block that is to be compressed.
 * @param[in] num The number of message blocks to be processed.
 */
void sm3_compress(u32 buf[8], const u8 *msg, u32 num);

/**
 * @brief Initializes the SM3 hash context structure.
 * This function initializes various members of the SM3 context structure to default values, preparing for subsequent hash calculations.
 * @param[in/out] ctx Pointer to the SM3 context structure.
*/
void sm3_init(sm3_ctx *ctx);

/**
 * @brief Updates the SM3 hash context with input data.
 * This function appends the provided input data to the current hash context and updates the hash state.
 * @param[in/out] ctx Pointer to the SM3 context structure.
 * @param[in] data Pointer to the input data to be processed.
 * @param[in] dlen Length of the input data, measured in bytes.
 */
void sm3_update(sm3_ctx *ctx, const void *data, u64 dlen);

/**
 * @brief Completes the SM3 hash computation and generates the final hash value.
 * This function processes the remaining data in the hash context and stores the computed hash value into the provided output buffer.
 * @param[in] ctx Pointer to the SM3 context structure.
 * @param[out] dgst Output buffer for storing the computed hash value. Should be at least 32 bytes to accommodate the SM3 hash value.
 */
void sm3_finish(sm3_ctx *ctx, u8 dgst[32]);

/**
 * @brief Computes the SM3 digest of a given message.
 * This function performs the entire SM3 hashing process and stores the final digest.
 * @param[out] digest The output buffer where the computed hash will be stored.
 * @param[in] msg A pointer to the input message that is to be hashed.
 * @param[in] mlen The length of the message to be hashed, measured in bytes.
 */
void sm3(u8 digest[32], const void *msg, u64 mlen);

#endif //SM_SM3_H

