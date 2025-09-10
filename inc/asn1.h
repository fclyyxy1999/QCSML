#ifndef QCSML_ASN1_H
#define QCSML_ASN1_H
#include <string.h>
#include "common.h"

/**
 * @file asn1.h
 * @brief This header file contains the functions for ASN.1 encoding and decoding.
 *
 * Functions:
 * -asn1_length_to_der(): Convert an ASN.1 length to DER-encoded format.
 * -asn1_length_from_der(): Convert a DER-encoded ASN.1 length to its original format.
 * -asn1_tag_to_der(): Convert an ASN.1 tag to DER-encoded format.
 * -asn1_tag_from_der(): Convert a DER-encoded ASN.1 tag to its original format.
 * -asn1_boolean_to_der(): Convert an ASN.1 boolean to DER-encoded format.
 * -asn1_boolean_from_der(): Convert a DER-encoded ASN.1 boolean to its original format.
 * -asn1_integer_to_der(): Convert an ASN.1 integer to DER-encoded format.
 * -asn1_integer_from_der(): Convert a DER-encoded ASN.1 integer to its original format.
 * -asn1_bitstring_to_der(): Convert an ASN.1 bit string to DER-encoded format.
 * -asn1_bitstring_from_der(): Convert a DER-encoded ASN.1 bit string to its original format.
 * -asn1_octets_to_der(): Convert an ASN.1 octet string to DER-encoded format.
 * -asn1_octets_from_der(): Convert a DER-encoded ASN.1 octet string to its original format.
 * -asn1_sequence_to_der(): Convert an ASN.1 sequence to DER-encoded format.
 * -asn1_sequence_from_der(): Convert a DER-encoded ASN.1 sequence to its original format.
 *

 */

// Enumeration defining the ASN.1 tag types
enum ASN1_TAG {
    TAG_BOOLEAN             = 1,    /**< Boolean type */
    TAG_INTEGER             = 2,    /**< Integer type */
    TAG_BIT_STRING          = 3,    /**< Bit string type */
    TAG_OCTET_STRING        = 4,    /**< Octet string type */
    TAG_NULL                = 5,    /**< Null type */
    TAG_OBJECT_IDENTIFIER   = 6,    /**< Object identifier type */
    TAG_ObjectDescriptor    = 7,    /**< Object descriptor type */
    TAG_EXTERNAL            = 8,    /**< External type */
    TAG_REAL                = 9,    /**< Real type */
    TAG_ENUMERATED          = 10,   /**< Enumerated type */
    TAG_SEQUENCE            = 0x30, /**< Sequence type */
    TAG_SET                 = 0x31  /**< Set type */
};

/**
 * @brief Calculate the DER encoding length for a given length value.
 * @param len The length value to encode.
 * @param dst Pointer to the destination buffer for the DER-encoded length.
 * @param dstlen Pointer to the length of the destination buffer.
 * @return 0 <= On success, the DER-encoded length is stored in dst and its length is stored in dstlen.
 * @return -1 If an error occurs.
 */
int asn1_length_to_der(u64 len, u8 **dst, u64 *dstlen);

/**
 * @brief Calculate the length value from DER-encoded length data.
 * @param len Pointer to the variable in which the length value will be stored.
 * @param src Pointer to the DER-encoded length data.
 * @param srclen Pointer to the length of the DER-encoded length data.
 * @return 0 <= On success, the length value is stored in len.
 * @return -1 If an error occurs.
 */
int asn1_length_from_der(u64 *len, const u8 **src, u64 *srclen);

/**
 * @brief Encode a boolean value to DER format.
 * @param val The boolean value to encode (0 for `false` and non-zero for `true`).
 * @param dst Pointer to the destination buffer for the DER-encoded boolean.
 * @param dstlen Pointer to the length of the destination buffer.
 * @return 0 <= On success, the DER-encoded boolean is stored in dst and its length is stored in dstlen.
 * @return -1 If an error occurs.
 */
int asn1_boolean_to_der(int val, u8 **dst, u64 *dstlen);

/**
 * @brief Decode a boolean value from DER format.
 * @param val Pointer to the variable in which the decoded boolean value will be stored.
 * @param src Pointer to the DER-encoded boolean data.
 * @param srclen Pointer to the length of the DER-encoded boolean data.
 * @return 0 <= On success, the decoded boolean value is stored in val.
 * @return -1 If an error occurs.
 */
int asn1_boolean_from_der(int *val, const u8 **src, u64 *srclen);

/**
 * @brief Encode an integer value to DER format.
 * @param num Pointer to the integer value to encode.
 * @param numlen The length of the integer value.
 * @param dst Pointer to the destination buffer for the DER-encoded integer.
 * @param dstlen Pointer to the length of the destination buffer.
 * @return 0 <= On success, the DER-encoded integer is stored in dst and its length is stored in dstlen.
 * @return -1 If an error occurs.
 */
int asn1_integer_to_der(const u8 *num, u64 numlen, u8 **dst, u64 *dstlen);

/**
 * @brief Decode an integer value from DER format.
 * @param num Pointer to the variable in which the decoded integer value will be stored.
 * @param numlen Pointer to the length of the decoded integer value.
 * @param src Pointer to the DER-encoded integer data.
 * @param srclen Pointer to the length of the DER-encoded integer data.
 * @return 0 <= On success, the decoded integer value is stored in num and its length is stored in numlen.
 * @return -1 If an error occurs.
 */
int asn1_integer_from_der(u8 *num, u64 *numlen, const u8 **src, u64 *srclen);

/**
 * @brief Encode a bit string to DER format.
 * @param bits Pointer to the bit string to encode.
 * @param nbits The length of the bit string.
 * @param dst Pointer to the destination buffer for the DER-encoded bit string.
 * @param dstlen Pointer to the length of the destination buffer.
 * @return 0 <= On success, the DER-encoded bit string is stored in dst and its length is stored in dstlen.
 * @return -1 If an error occurs.
 */
int asn1_bit_string_to_der(const u8 *bits, u64 nbits, u8 **dst, u64 *dstlen);

/**
 * @brief Decode a bit string from DER format.
 * @param bits Pointer to the variable in which the decoded bit string will be stored.
 * @param nbits Pointer to the length of the decoded bit string.
 * @param src Pointer to the DER-encoded bit string data.
 * @param srclen Pointer to the length of the DER-encoded bit string data.
 * @return 0 <= On success, the decoded bit string is stored in bits and its length is stored in nbits.
 * @return -1 If an error occurs.
 */
int asn1_bit_string_from_der(u8 *bits, u64 *nbits, const u8 **src, u64 *srclen);

/**
 * @brief Encode an octet string to DER format.
 * @param octets Pointer to the octet string to encode.
 * @param noctets The length of the octet string.
 * @param dst Pointer to the destination buffer for the DER-encoded octet string.
 * @param dstlen Pointer to the length of the destination buffer.
 * @return 0 <= On success, the DER-encoded octet string is stored in dst and its length is stored in dstlen.
 * @return -1 If an error occurs.
 */
int asn1_octets_to_der(const u8 *octets, u64 noctets, u8 **dst, u64 *dstlen);

/**
 * @brief Decode an octet string from DER format.
 * @param octets Pointer to the variable in which the decoded octet string will be stored.
 * @param noctets Pointer to the length of the decoded octet string.
 * @param src Pointer to the DER-encoded octet string data.
 * @param srclen Pointer to the length of the DER-encoded octet string data.
 * @return 0 <= On success, the decoded octet string is stored in octets and its length is stored in noctets.
 * @return -1 If an error occurs.
 */
int asn1_octets_from_der(u8 *octets, u64 *noctets, const u8 **src, u64 *srclen);

/**
 * @brief Encode a sequence to DER format.
 * @param data Pointer to the sequence to encode.
 * @param datalen The length of the sequence.
 * @param dst Pointer to the destination buffer for the DER-encoded sequence.
 * @param dstlen Pointer to the length of the destination buffer.
 * @return 0 <= On success, the DER-encoded sequence is stored in dst and its length is stored in dstlen.
 * @return -1 If an error occurs.
 */
int asn1_sequence_to_der(const u8 *data, u64 datalen, u8 **dst, u64 *dstlen);

/**
 * @brief Decode a sequence from DER format.
 * @param data Pointer to the variable in which the decoded sequence will be stored.
 * @param datalen Pointer to the length of the decoded sequence.
 * @param src Pointer to the DER-encoded sequence data.
 * @param srclen Pointer to the length of the DER-encoded sequence data.
 * @return 0 <= On success, the decoded sequence is stored in data and its length is stored in datalen.
 * @return -1 If an error occurs.
 */
int asn1_sequence_from_der(u8 *data, u64 *datalen, const u8 **src, u64 *srclen);

#endif // QCSML_ASN1_H