/**
* @file sm4.h
 * @brief This file contains declarations of structures and functions related to SM4 algorithm.
 * @date 2025-01-07
 * @author RunChen Zhao
 * @email zrc1024@foxmail.com
 * @version 2.0
 */

#ifndef SM_SM4_H
#define SM_SM4_H

#include "common.h"

#define SM4_BLOCKSIZE     16
#define SM4_BLOCKSIZE_16  256
#define SM4_ROUNDS        32

extern const u32 kt[4][256];
extern const u32 rt[4][256];

typedef struct SM4Context {
    u32 rk[SM4_ROUNDS];         /// Round keys for SM4 encryption/decryption
    u8  iv[SM4_BLOCKSIZE];      /// not ready yet
} sm4_ctx;
/**
 * @brief SM4 Set the encryption and decryption key.
 *
 * @param [OUT] rk        Round keys for SM4 encryption/decryption
 * @param [in] key        Input key for SM4 encryption/decryption
 * @return @todo not ready yet
 */
void sm4_setkey_core(u32 *rk, const u8 *key);

/**
 * @brief SM4 encryption core function.
 *
 * @param [OUT] out      Output buffer for encrypted data
 * @param [IN] in        Input buffer for data to be encrypted
 * @param [IN] rk        Round keys for SM4 encryption
 */
void sm4_enc_core(u8 *out, const u8 *in, const u32 *rk);

/**
 * @brief SM4 decryption core function.
 *
 * @param [OUT] out      Output buffer for decrypted data
 * @param [IN] in        Input buffer for data to be decrypted
 * @param [IN] rk        Round keys for SM4 decryption
 */
void sm4_dec_core(u8 *out, const u8 *in, const u32 *rk);

/**
 * @brief SM4 Set the encryption and decryption key.
 *
 * @param [IN] ctx       SM4 context
 * @param [IN] key       Key
 * @param [IN] keyLen    Key length
 * @return @todo not ready yet
 */
void sm4_setkey(sm4_ctx *ctx, const u8 *key, u32 keyLen);

/**
 * @brief SM4 encryption. The data length must be an integer multiple of 16.
 *
 * @param [IN] ctx      SM4 context
 * @param [IN] in       Data to be encrypted
 * @param [OUT] out     Encrypted data
 * @param [IN] length   Data length
 * @return @todo not ready yet
 */
void sm4_encrypt(const sm4_ctx *ctx, const u8 *in, u8 *out, u32 length);

/**
 * @brief SM4 decryption. The data length must be an integer multiple of 16.
 *
 * @param [IN] ctx      SM4 context
 * @param [IN] in       Data to be decrypted
 * @param [OUT] out     Decrypted Data
 * @param [IN] length   Data length
 * @return @todo not ready yet
 */
void sm4_decrypt(const sm4_ctx *ctx, const u8 *in, u8 *out, u32 length);


#endif //SM_SM4_H
