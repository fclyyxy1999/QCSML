/**
 * @file sm2.h
 * @brief This file contains the structure and function declarations for key management, sign, verify, encrypti and decrypt for SM2.
 * For the declarations of structures and functions for underlying finite field operations and elliptic curve point operations @see sm2_core.h.
 * @date 2025-01-07
 * @author RunChen Zhao
 * @email zrc1024@foxmail.com
 * @version 2.0
 */

#ifndef QCSML_SM2_H
#define QCSML_SM2_H

#include "sm2_core.h"
#include "sm3.h"

/**
 * @brief Defines the public key type in the SM2 algorithm, which is essentially a point structure.
 * @note The pubkey is stored as the big-endian byte array and used for upper-layer output and network transmission.
 */
typedef sm2_point sm2_pubkey;

/**
 * @brief Defines the private key structure in the SM2 algorithm.
 * @note The [private] is stored as the big-endian byte array and used for upper-layer output and network transmission.
 * The [pubkey], [d], and [d_inv] are also stored in this structure to save the overhead of repeated computations.
 * This structure should always be initialised with the sm2_keygen(), sm2_prikey_from_str(), and sm2_prikey_from_bytes() functions,
 * as they all call sm2_prikey_check() to compute the [pubkey], [d], and [d_inv].
 */
typedef struct SM2PrivateKey{
    sm2_bn      prikey;             ///< Private key
    sm2_pubkey  pubkey;             ///< Public key
    sm2_fp      d;                  ///< Private key in the present of sm2_fp, it is used for underlying calculations
    sm2_fp      d_inv;              ///< d_inv = (1 + d)^(-1)
} sm2_prikey;


/// Structure representing an SM2 signature.
typedef struct SM2_Signature{
    sm2_bn r;                       ///< r value of the signature
    sm2_bn s;                       ///< s value of the signature
} sm2_signature;


/// SM2 signing context structure.
typedef struct SM2SignContext{
    const sm2_prikey*   prikey;     ///< Private key used for signing
    sm3_ctx*            sm3_ctx;    ///< Hash context for SM3
    sm3_digest          dgst;       ///< Message digest
    sm2_signature       sig;        ///< SM2 signature
} sm2_sign_ctx;


/// SM2 verify context structure.
typedef struct SM2VerifyContext{
    const sm2_pubkey*   pubkey;     ///< Public key used for verification
    sm3_ctx*            sm3_ctx;    ///< Hash context for SM3
    sm3_digest          dgst;       ///< Message digest
} sm2_verify_ctx;


/// Structure representing the SM2 ciphertext.
typedef struct SM2Cipertext{
    sm2_point  C1;                  ///< Pointer to the ciphertext data C1
    sm3_digest C3;                  ///< Pointer to the ciphertext data C3
    u8 *C2;                         ///< Pointer to the ciphertext data C2
    u32 clen;                       ///< Length of the ciphertext
} sm2_ciptext;

/**
 * @brief Generates a key pair for the SM2 algorithm.
 * @param[out] prikey Pointer to the structure where the private key will be stored.
 * @return Returns 0 on success, or an error code on failure.
 */
int sm2_keygen(sm2_prikey *prikey);

/**
 * @brief Checks the validity of a private key.
 * @param[in/out] prikey Pointer to the private key structure to check.
 */
void sm2_prikey_check(sm2_prikey *prikey);

/**
 * @brief Converts a string representation to an SM2 private key.
 * @param[out] prikey Pointer to the structure where the private key will be stored.
 * @param[in] s String representation of the private key.
 */
void sm2_prikey_from_str(sm2_prikey *prikey, const char *s);

/**
 * @brief Converts a byte array to an SM2 private key.
 * @param[out] prikey Pointer to the structure where the private key will be stored.
 * @param[in] b Byte array representation of the private key.
 * @param[in] blen Length of the byte array.
 */
void sm2_prikey_from_bytes(sm2_prikey *prikey, const u8 *b, u32 blen);

/**
 * @brief Converts a string representation to an SM2 public key.
 * @param[out] pubkey Pointer to the structure where the public key will be stored.
 * @param[in] s String representation of the public key.
 */
void sm2_pubkey_from_str(sm2_pubkey *pubkey, const char *s);

/**
 * @brief Converts a byte array to an SM2 public key.
 * @param[out] pubkey Pointer to the structure where the public key will be stored.
 * @param[in] b Byte array representation of the public key.
 * @param[in] blen Length of the byte array.
 */
void sm2_pubkey_from_bytes(sm2_pubkey *pubkey, const u8 *b, u32 blen);

/**
 * @brief Core function for signing a message using SM2.
 * @param[out] sig Pointer to the structure where the generated signature will be stored.
 * @param[in] prikey Pointer to the private key used for signing.
 * @param[in] dgst Pointer to the message digest to be signed.
 */
void sm2_sign_core(sm2_signature *sig, const sm2_prikey *prikey, sm3_digest dgst);

/**
 * @brief Core function for verifying an SM2 signature.
 * @param[in] sig Pointer to the signature to be verified.
 * @param[in] publickey Pointer to the public key used for verification.
 * @param[in] dgst Pointer to the message digest that was signed.
 * @return Returns 1 if the signature is passed, or 0 if invalid.
 */
int sm2_verify_core(const sm2_signature *sig, const sm2_pubkey *publickey, sm3_digest dgst);

/**
 * @brief Initialize the SM2 signing context.
 * @param[in/out] ctx Pointer to the signing context structure to be initialized.
 * @param[in] prikey Pointer to the private key used for signing.
 * @param[in] id Identifier used for signing.
 * @param[in] idlen Length of the identifier.
 */
void sm2_sign_init(sm2_sign_ctx *ctx, const sm2_prikey *prikey, const char *id, u32 idlen);

/**
 * @brief Update the signing context with additional message data.
 * @param[in/out] ctx Pointer to the signing context structure.
 * @param[in] msg Pointer to the additional message data.
 * @param[in] msglen Length of the message data.
 */
void sm2_sign_update(const sm2_sign_ctx *ctx, const void *msg, u64 msglen);

/**
 * @brief Finalize the signing process and output the signature.
 * @param[in] ctx Pointer to the signing context structure.
 * @param[out] sig Pointer to the buffer where the signature will be stored.
 * @param[out] siglen Length of the signature buffer.
 */
void sm2_sign_finish(sm2_sign_ctx *ctx, u8 *sig, u64 *siglen);

/**
 * @brief Sign a message using SM2 and produce a signature.
 * @param[in] prikey Pointer to the private key used for signing.
 * @param[out] sig Pointer to the buffer where the signature will be stored.
 * @param[out] siglen Length of the signature buffer.
 * @param[in] id Identifier used for signing.
 * @param[in] idlen Length of the identifier.
 * @param[in] msg Pointer to the message to be signed.
 * @param[in] msglen Length of the message.
 */
void sm2_sign(const sm2_prikey *prikey, u8 *sig, u64 *siglen,  const char *id, u32 idlen, const void *msg, u64 msglen);

/**
 * @brief Initialize the SM2 signature verification context.
 * @param[in/out] ctx Pointer to the verification context structure to be initialized.
 * @param[in] pubkey Pointer to the public key used for verification.
 * @param[in] id Identifier used for verification.
 * @param[in] idlen Length of the identifier.
 * @return Returns 1 on success, or an error code on failure.
 */
int sm2_verify_init(sm2_verify_ctx *ctx, const sm2_pubkey *pubkey, const char *id, u64 idlen);

/**
 * @brief Update the verification context with message data for signature verification.
 * @param[in/out] ctx Pointer to the verification context structure.
 * @param[in] msg Pointer to the message data.
 * @param[in] msglen Length of the message data.
 * @return Returns 1 on success, or an error code on failure.
 */
int sm2_verify_update(sm2_verify_ctx *ctx, const void *msg, u64 msglen);

/**
 * @brief Finalize the signature verification process and check the signature.
 * @param[in] ctx Pointer to the verification context structure.
 * @param[in] sig Pointer to the signature to be verified.
 * @param[in] siglen Length of the signature.
 * @return Returns 1 if the signature is valid, or an error code if invalid.
 */
int sm2_verify_finish(sm2_verify_ctx *ctx, const u8 *sig, u64 siglen);

/**
 * @brief Verify a signature using the SM2 algorithm.
 * @param[in] pubkey Pointer to the public key used for verification.
 * @param[in] sig Pointer to the signature to be verified.
 * @param[in] siglen Length of the signature.
 * @param[in] id Identifier used for verification.
 * @param[in] idlen Length of the identifier.
 * @param[in] msg Pointer to the signed message.
 * @param[in] msglen Length of the message.
 * @return Returns 1 if the signature is valid, or an error code if invalid.
 */
int sm2_verify(const sm2_pubkey *pubkey, const u8 *sig, u64 siglen, const char *id,
               u64 idlen, const void *msg, u64 msglen);

/**
 * @brief Convert an SM2 signature to DER-encoded format.
 * @param[in] sig The signature to be converted.
 * @param[out] dst The output buffer where the DER-encoded signature will be stored.
 * @param[out] dstl The length of the output buffer.
 */
void sm2_sig_to_der(const sm2_signature* sig, u8 **dst, u64 *dstl);

/**
 * @brief Convert a DER-encoded SM2 signature to its original format.
 * @param[out] sig Pointer to store the original signature.
 * @param[in] src Pointer to the DER-encoded signature.
 * @param[in] srcl Length of the DER-encoded signature.
 */
void sm2_sig_from_der(sm2_signature *sig, const u8 **src, u64 *srcl);

/**
 * @brief Derive the SM2 key pair from a given point.
 * @param[out] output Pointer to store the derived output.
 * @param[in] p The point used for key derivation.
 * @param[in] klen Length of the key to be derived.
 */
void sm2_keydepair(u8 *output, const sm2_point* p, u32 klen);

/**
 * @brief Core function for encrypting a message using SM2.
 * @param[out] ciphertext Pointer to the structure where the ciphertext will be stored.
 * @param[in] pubkey Pointer to the public key used for encryption.
 * @param[in] msg Pointer to the message to be encrypted.
 * @param[in] msglen Length of the message to be encrypted.
 */
void sm2_encrypt_core(sm2_ciptext *ciphertext, const sm2_pubkey *pubkey, const char *msg, u64 msglen);

/**
 * @brief Core function for decrypting a ciphertext using SM2.
 * @param[out] pt Pointer to the buffer where the decrypted plaintext will be stored.
 * @param[out] ptlen Length of the plaintext buffer.
 * @param[in] prikey Pointer to the private key used for decryption.
 * @param[in] ciptext Pointer to the ciphertext to be decrypted.
 * @return Returns 0 on success, or an error code on failure.
 */
int sm2_decrypt_core(u8 *pt, u64 *ptlen, const sm2_prikey *prikey, const sm2_ciptext *ciptext);

/**
 * @brief Convert an SM2 ciphertext to DER-encoded format.
 * @param[in] ct The ciphertext to be converted.
 * @param[out] dst The output buffer to store the DER-encoded ciphertext.
 * @param[out] dstl The length of the output buffer.
 */
void sm2_ciptext_to_der(const sm2_ciptext* ct, u8 **dst, u64 *dstl);

/**
 * @brief Convert a DER-encoded SM2 ciphertext to its original format.
 * @param[out] ct The output buffer to store the converted ciphertext.
 * @param[in] src The DER-encoded ciphertext.
 * @param[in] srcl The length of the DER-encoded ciphertext.
 */
void sm2_ciptext_from_der(sm2_ciptext *ct, const u8 **src, u64 *srcl);

/**
 * @brief Encrypt a plaintext message using the SM2 algorithm.
 * @param[in] pubkey Pointer to the public key used for encryption.
 * @param[out] ct Pointer to the buffer where the ciphertext will be stored.
 * @param[out] ctlen Pointer to the variable that stores the length of the ciphertext.
 * @param[in] pt Pointer to the plaintext message to be encrypted.
 * @param[in] ptlen Length of the plaintext message.
 */
void sm2_encrypt(const sm2_pubkey *pubkey, u8 *ct, u64 *ctlen, const void* pt, u64 ptlen);

/**
 * @brief Decrypt a ciphertext using the SM2 algorithm.
 * @param[in] prikey Pointer to the private key used for decryption.
 * @param[out] pt Pointer to the buffer where the decrypted plaintext will be stored.
 * @param[out] ptlen Pointer to the variable that stores the length of the plaintext.
 * @param[in] ct Pointer to the ciphertext to be decrypted.
 * @param[in] ctlen Length of the ciphertext.
 * @return Returns 0 on success, or an error code on failure.
 */
int sm2_decrypt(const sm2_prikey *prikey, u8 *pt, u64 *ptlen, const void* ct, u64 ctlen);

#endif // QCSML_SM2_H