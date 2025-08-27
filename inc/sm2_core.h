/**
 * @file sm2_core.h
 * @brief This file contains the declarations of structures and functions for underlying finite field operations and elliptic curve point operations
 * of SM2.
 * @date 2025-01-07
 * @author RunChen Zhao
 * @email zrc1024@foxmail.com
 * @version 2.0
 */

#ifndef SM2_CORE_H
#define SM2_CORE_H

#include "common.h"
#include "rand.h"
#include "asn1.h"

/**
 * @brief The type representing a 256-bit number in finite prime field.
 * @note The number is stored as a big-endian byte array for upper-layer output and network transmission
 */
typedef u8 sm2_bn[32];

/**
 * @brief The type representing a 256-bit number in finite prime field.
 * @note The number is stored in little-endian order and used for underlying (mathematical) operations.
 */
typedef u32 sm2_fp[8];


/// The type representing a 512-bit number to store the multiplication product of sm2_fp.
typedef u32 sm2_dfp[16];


/// The type representing a Non-Adjacent Form (NAF) for efficient scalar multiplication
typedef i8  sm2_naf[257];


/// The constant value representing zero.
extern const sm2_fp sm2_zero;
/// The constant value representing one.
extern const sm2_fp sm2_one;
/// The constant value representing the prime number p.
extern const sm2_fp sm2_p;
/// The constant value representing the order n of the base point G.
extern const sm2_fp sm2_n;
/// The constant value representing the coefficient a of the elliptic curve.
extern const sm2_fp sm2_a;
/// The constant value representing the coefficient b of the elliptic curve.
extern const sm2_fp sm2_b;
/// The constant value representing the value (p + 1) / 2.
extern const sm2_fp sm2_s;


/**
 * @brief The structure representing a point of SM2.
 * @note The point is stored as two sm2_bn, i.e., the big-endian byte array and used for upper-layer output and network transmission
 */
typedef struct SM2Point{
    sm2_bn x;   ///< x-coordinate of the point
    sm2_bn y;   ///< y-coordinate of the point
} sm2_point;

/**
 * @brief The structure representing an affine point on the elliptic curve.
 * @note This structure is used for underlying (mathematical) operations.
 */
typedef struct SM2AffinePoint{
    sm2_fp x;   ///< x-coordinate of the affine point
    sm2_fp y;   ///< y-coordinate of the affine point
} sm2_apoint;

/**
 * @brief The structure representing a jacobian point on the elliptic curve.
 * @note This structure is used for underlying (mathematical) operations.
 */
typedef struct SM2JacobianPoint{
    sm2_fp x;   ///< x-coordinate of the jacobian point
    sm2_fp y;   ///< y-coordinate of the jacobian point
    sm2_fp z;   ///< z-coordinate of the jacobian point
} sm2_jpoint;

/**
 * @brief Sets the value of one number to another.
 * @param [out] r The result number.
 * @param [in] a The number to copy from.
 */
void sm2_fp_set(sm2_fp r, const sm2_fp a);

/**
 * @brief Checks whether the number is odd.
 * @param [in] a The number to check.
 * @return 1 if odd, 0 otherwise.
 */
int sm2_fp_is_odd(const sm2_fp a);

/**
 * @brief Checks whether the number is even.
 * @param [in] a The number to check.
 * @return 1 if even, 0 otherwise.
 */
int sm2_fp_is_even(const sm2_fp a);

/**
 * @brief Checks whether the number is zero.
 * @param [in] a The number to check.
 * @return 1 if zero, 0 otherwise.
 */
int sm2_fp_is_zero(const sm2_fp a);

/**
 * @brief  Checks whether the number is non-zero.
 * @param [in] a The number to check.
 * @return 1 if not zero, 0 otherwise.
 */
int sm2_fp_non_zero(const sm2_fp a);

/**
 * @brief Checks whether two numbers are equal.
 * @param [in] a The first number.
 * @param [in] b The second number.
 * @return 1 if equal, 0 otherwise.
 */
int sm2_fp_equ(const sm2_fp a, const sm2_fp b);

/**
 * @brief Compares two numbers.
 * @param [in] a The first number.
 * @param [in] b The second number.
 * @return 1 if a >= b, 0 otherwise.
 */
int sm2_fp_cmp(const sm2_fp a, const sm2_fp b);

/**
 * @brief Computes addition modulo sm2_p, i.e., r ≡ a + b mod sm2_p.
 * @param [out] r The result number.
 * @param [in] a The first number.
 * @param [in] b The second number.
 */
void sm2_fp_add(sm2_fp r, const sm2_fp a, const sm2_fp b);

/**
 * @brief Computes subtraction modulo sm2_p, i.e., r ≡ a - b mod sm2_p.
 * @param [out] r The result number.
 * @param [in] a The minuend.
 * @param [in] b The subtrahend.
 */
void sm2_fp_sub(sm2_fp r, const sm2_fp a, const sm2_fp b);

/**
 * @brief Computes additive inverse modulo sm2_p, i.e., r ≡ -a mod sm2_p.
 * @param [out] r The result number.
 * @param [in] a The number to negate.
 */
void sm2_fp_neg(sm2_fp r, const sm2_fp a);

/**
 * @brief Computes halving modulo sm2_p, i.e., r ≡ a/2 mod sm2_p.
 * @param [out] r The result number.
 * @param [in] a The number to halve.
 */
void sm2_fp_haf(sm2_fp r, const sm2_fp a);

/**
 * @brief Computes doubling modulo sm2_p, i.e., r ≡ 2a mod sm2_p.
 * @param [out] r The result number.
 * @param [in] a The number to double.
 */
void sm2_fp_dbl(sm2_fp r, const sm2_fp a);

/**
 * @brief Computes tripling modulo sm2_p, i.e., r ≡ 3a mod sm2_p.
 * @param [out] r The result number.
 * @param [in] a The number to triple.
 */
void sm2_fp_tri(sm2_fp r, const sm2_fp a);

/**
 * @brief Computes the reduction modulo sm2_p, i.e., r ≡ c mod sm2_p.
 * @param [out] r The result number.
 * @param [in] c The dnumber to recover from.
 */
void sm2_fp_rec(sm2_fp r, const sm2_dfp c);

/**
 * @brief Computes multiplication modulo sm2_p, i.e., r ≡ a * b mod sm2_p.
 * @param [out] r The result number.
 * @param [in] a The first number.
 * @param [in] b The second number.
 */
void sm2_fp_mul(sm2_fp r, const sm2_fp a, const sm2_fp b);

/**
 * @brief Computes squaring modulo sm2_p, i.e., r ≡ a^2 mod sm2_p.
 * @param [out] r The result number.
 * @param [in] a The number to square.
 */
void sm2_fp_sqr(sm2_fp r, const sm2_fp a);

/**
 * @brief Computes cubing modulo sm2_p, i.e., r ≡ a^3 mod sm2_p.
 * @param [out] r The result number.
 * @param [in] a The number to cube.
 */
void sm2_fp_cub(sm2_fp r, const sm2_fp a);

/**
 * @brief Computes multiplicative inverse modulo sm2_p, i.e., r ≡ a^-1 mod sm2_p.
 * @param [out] r The result number.
 * @param [in] q The number to invert.
 */
void sm2_fp_inv(sm2_fp r, const sm2_fp q);

/**
 * @brief Computes addition modulo sm2_n, i.e., r ≡ a + b mod sm2_n.
 * @param [out] r The result number.
 * @param [in] a The first number.
 * @param [in] b The second number.
 */
void sm2_fn_add(sm2_fp r, const sm2_fp a, const sm2_fp b);

/**
 * @brief Computes subtraction modulo sm2_n, i.e., r ≡ a - b mod sm2_n.
 * @param [out] r The result number.
 * @param [in] a The minuend.
 * @param [in] b The subtrahend.
 */
void sm2_fn_sub(sm2_fp r, const sm2_fp a, const sm2_fp b);

/**
 * @brief Computes the reduction modulo sm2_n, i.e., r ≡ a mod sm2_n.
 * @param [out] r The result number.
 * @param [in] a The dnumber to recover from.
 */
void sm2_fn_rec(sm2_fp r, const sm2_dfp a);

/**
 * @brief Computes multiplication modulo sm2_n, i.e., r ≡ a * b mod sm2_n.
 * @param [out] r The result number.
 * @param [in] a The first number.
 * @param [in] b The second number.
 */
void sm2_fn_mul(sm2_fp r, const sm2_fp a, const sm2_fp b);

/**
 * @brief Computes multiplicative inverse modulo sm2_n, i.e., r ≡ a^-1 mod sm2_n.
 * @param [out] r The result number.
 * @param [in] q The number to invert.
 */
void sm2_fn_inv(sm2_fp r, const sm2_fp q);

/**
 * @brief Generates a random number in the range [1, n-1].
 * @param [out] r The output number.
 */
void sm2_fn_rand(sm2_fp r);
/**
 * @brief Converts a number to its hex string representation.
 * @param [in] n The number to convert.
 * @param [out] s The string buffer to store the result.
 */
void sm2_fp_to_str(const sm2_fp n, char* s);

/**
 * @brief Fills a number from a hex string representation.
 * @param [out] n The result number.
 * @param [in] s The string to convert from.
 */
void sm2_fp_from_str(sm2_fp n, const char* s);

/**
 * @brief Converts a number to its byte array.
 * @param [in] n The number to convert.
 * @param [out] b The byte array to store the result.
 */
void sm2_fp_to_bn(const sm2_fp n, sm2_bn b);

/**
 * @brief Fills a number from a byte array.
 * @param [in] b The byte array to convert from.
 * @param [out] n The result number.
 */
void sm2_bn_to_fp(const sm2_bn b, sm2_fp n);

/**
 * @brief Converts a byte array to its hex string representation.
 * @param [in] b The byte array to convert.
 * @param [out] s The string buffer to store the result.
 */
void sm2_bn_to_str(const sm2_bn b, char* s);

/**
 * @brief Fills a byte array from a hex string representation.
 * @param [out] b The result byte array.
 * @param [in] s The string to convert from.
 */
void sm2_bn_from_str(sm2_bn b, const char* s);

/**
 * @brief Computes the Non-Adjacent Form (NAF) representation of a number.
 * @param [out] r The result NAF array.
 * @param [in] w The width of NAF.
 * @param [in] n The number to compute the NAF for.
 */
void sm2_fp_naf(sm2_naf r, u8 w, const sm2_fp n);

/**
 * @brief Computes the Non-Adjacent Form (NAF) representation of a number.
 * @param [out] K The result NAF array.
 * @param [in] k The number to compute the NAF for.
 * @note it only used in sm2_jpoint_gen
 */
void sm2_fp_nafp(i8 K[52], const sm2_fp k);

/**
 * @brief Converts a string representation of a point to an SM2Point.
 * @param [out] p Pointer to the destination SM2Point.
 * @param [in] s String representation of the point.
 */
void sm2_point_from_str(sm2_point* p, const char *s);

/**
 * @brief Converts a byte array to an SM2Point.
 * @param [out] p Pointer to the destination SM2Point.
 * @param [in] b Byte array representation of the point.
 * @param [in] blen Length of the byte array.
 */
void sm2_point_from_bytes(sm2_point *p, const u8 *b, u32 blen);

/**
 * @brief Multiplies a scalar(number) with the base point on the sm2 curve, i.e., r = [k]G.
 * @param [out] r Pointer to the resulting SM2Point.
 * @param [in] k Scalar for multiplication.
 */
void sm2_point_gen(sm2_point *r, const sm2_fp k);

/**
 * @brief Multiplies a scalar(number) with the given point on the sm2 curve, i.e., r = [k]p.
 * @param [out] r Pointer to the resulting SM2Point.
 * @param [in] k Scalar for multiplication.
 * @param [in] p Pointer to the SM2Point to be multiplied.
 */
void sm2_point_mul(sm2_point *r, const sm2_fp k, const sm2_point *p);

/**
 * @brief Checks if a point is on the elliptic curve.
 * @param [in] r Pointer to the SM2Point to be checked.
 * @return Returns 1 if the point is on the curve, 0 otherwise.
 */
int sm2_point_is_on_curve(const sm2_point *r);

/**
 * @brief Converts a point from SM2Point to SM2AffinePoint.
 * @param [in] src Pointer to the source SM2Point.
 * @param [out] p Pointer to the destination SM2AffinePoint.
 */
void sm2_point_to_apoint(const sm2_point *src, sm2_apoint *p);

/**
 * @brief Converts a point from SM2AffinePoint to SM2Point.
 * @param [in] p Pointer to the source SM2AffinePoint.
 * @param [out] dst Pointer to the destination SM2Point.
 */
void sm2_apoint_to_point(const sm2_apoint* p, sm2_point *dst);

/**
 * @brief Converts a string representation to an SM2AffinePoint.
 * @param [out] p Pointer to the destination SM2AffinePoint.
 * @param [in] s String representation of the affine point.
 */
void sm2_apoint_from_str(sm2_apoint *p, const char *s);

/**
 * @brief Converts a byte array to an SM2AffinePoint.
 * @param [out] p Pointer to the destination SM2AffinePoint.
 * @param [in] b Byte array representation of the affine point.
 * @param [in] blen Length of the byte array.
 */
void sm2_apoint_from_bytes(sm2_apoint *p, const u8 *b, u32 blen);

/**
 * @brief Sets the x and y coordinates of an SM2AffinePoint.
 * @param [out] p Pointer to the destination SM2AffinePoint.
 * @param [in] x X coordinate to set.
 * @param [in] y Y coordinate to set.
 */
void sm2_apoint_set(sm2_apoint *p, const sm2_fp x, const sm2_fp y);

/**
 * @brief Checks if an affine point represents the point at infinity.
 * @param [in] r Pointer to the SM2AffinePoint to be checked.
 * @return Returns 1 if the point is at infinity, 0 otherwise.
 */
int sm2_apoint_at_infinity(const sm2_apoint *r);

/**
 * @brief Multiplies a scalar with a given affine point, i.e., r = [k]p.
 * @param [out] r Pointer to the resulting SM2AffinePoint.
 * @param [in] k Scalar for multiplication.
 * @param [in] p Pointer to the SM2AffinePoint to be multiplied.
 */
void sm2_apoint_mul(sm2_apoint *r, const sm2_fp k, const sm2_apoint *p);

/**
 * @brief Multiplies a scalar with a base affine point G, i.e., r = [k]G.
 * @param [out] r Pointer to the resulting SM2AffinePoint.
 * @param [in] k Scalar used for the generation.
 */
void sm2_apoint_gen(sm2_apoint *r, const sm2_fp k);

/**
 * @brief Multiplies two affine points together, i.e., r = [s]G + [t]q.
 * @param [out] r Pointer to the resulting SM2AffinePoint.
 * @param [in] s First scalar.
 * @param [in] t Second scalar.
 * @param [in] q Pointer to the SM2AffinePoint to be multiplied.
 */
void sm2_apoint_mutmul(sm2_apoint *r, sm2_fp s, sm2_fp t, const sm2_apoint *q);

/**
 * @brief Converts an affine point to a Jacobian point.
 * @param [in] p Pointer to the source SM2AffinePoint.
 * @param [out] r Pointer to the destination SM2JacobianPoint.
 */
void sm2_apoint_to_jpoint(const sm2_apoint *p, sm2_jpoint *r);

/**
 * @brief Converts a Jacobian point to an affine point.
 * @param [in] a Pointer to the source SM2JacobianPoint.
 * @param [out] r Pointer to the destination SM2AffinePoint.
 */
void sm2_jpoint_to_apoint(const sm2_jpoint *a, sm2_apoint *r);

/**
 * @brief Copies the content of one Jacobian point to another.
 * @param [out] p Pointer to the destination SM2JacobianPoint.
 * @param [in] q Pointer to the source SM2JacobianPoint.
 */
void sm2_jpoint_copy(sm2_jpoint *p, const sm2_jpoint *q);

/**
 * @brief Sets the coordinates of a Jacobian point.
 * @param [out] p Pointer to the destination SM2JacobianPoint.
 * @param [in] x X coordinate to set.
 * @param [in] y Y coordinate to set.
 * @param [in] z Z coordinate to set.
 */
void sm2_jpoint_set(sm2_jpoint *p, const sm2_fp x, const sm2_fp y, const sm2_fp z);

/**
 * @brief Sets a Jacobian point to the point at infinity.
 * @param [out] r Pointer to the SM2JacobianPoint to be set.
 */
void sm2_jpoint_init(sm2_jpoint *r);

/**
 * @brief Checks if a jpoint is at infinity.
 * @param [in] r The jpoint to check.
 * @return 1 if the jpoint is at infinity, 0 otherwise.
 */
int sm2_jpoint_at_infinity(const sm2_jpoint *r);

/**
 * @brief Converts a string representation to a Jacobian point.
 * @param [out] p Pointer to the destination SM2JacobianPoint.
 * @param [in] s String representation of the Jacobian point.
 */
void sm2_jpoint_from_str(sm2_jpoint *p, const char *s);

/**
 * @brief Adds two Jacobian points.
 * @param [out] r Pointer to the resulting SM2JacobianPoint.
 * @param [in] p Pointer to the first SM2JacobianPoint.
 * @param [in] q Pointer to the second SM2JacobianPoint.
 */
void sm2_jpoint_add(sm2_jpoint *r, const sm2_jpoint *p, const sm2_jpoint *q);

/**
 * @brief Subtracts one Jacobian point from another.
 * @param [out] r Pointer to the resulting SM2JacobianPoint.
 * @param [in] p Pointer to the minuend SM2JacobianPoint.
 * @param [in] q Pointer to the subtrahend SM2JacobianPoint.
 */
void sm2_jpoint_sub(sm2_jpoint *r, const sm2_jpoint *p, const sm2_jpoint *q);

/**
 * @brief Point addition, affine-Jacobian coordinates
 * @param [out] r The result of the addition.
 * @param [in] p The first jpoint.
 * @param [in] q The second point.
 */
void sm2_japoint_add(sm2_jpoint *r, const sm2_jpoint *p, const sm2_apoint *q);

/**
 * @brief Point subtraction, affine-Jacobian coordinates
 * @param [out] r The result of the subtraction.
 * @param [in] p The first jpoint.
 * @param [in] q The second point.
 */
void sm2_japoint_sub(sm2_jpoint *r, const sm2_jpoint *p, const sm2_apoint *q);

/**
 * @brief Doubles a Jacobian point.
 * @param [out] r Pointer to the resulting SM2JacobianPoint.
 * @param [in] a Pointer to the SM2JacobianPoint to be doubled.
 */
void sm2_jpoint_dou(sm2_jpoint *r, const sm2_jpoint *a);

/**
 * @brief Performs scalar multiplication on a Jacobian point.
 * @param [out] r Pointer to the resulting SM2JacobianPoint.
 * @param [in] m Scalar for multiplication.
 * @param [in] p Pointer to the SM2JacobianPoint to be multiplied.
 */
void sm2_jpoint_mdo(sm2_jpoint *r, u32 m, const sm2_jpoint *p);

/**
 * @brief Multiplies a scalar with a given Jacobian point.
 * @param [out] r Pointer to the resulting SM2JacobianPoint.
 * @param [in] k Scalar for multiplication.
 * @param [in] g Pointer to the SM2JacobianPoint to be multiplied.
 */
void sm2_jpoint_mul(sm2_jpoint *r, const sm2_fp k, const sm2_jpoint *g);
/**
 * @brief Multiplies a scalar with a base Jacobian point.
 * @param [out] r Pointer to the resulting SM2JacobianPoint.
 * @param [in] k Scalar used for the generation.
 */
void sm2_jpoint_gen(sm2_jpoint *r, const sm2_fp k);

#define Large_lookup_table_allowed 1

#if Large_lookup_table_allowed == 1
    extern const sm2_apoint sm2_pct[32][256];
#else
    extern const sm2_apoint fpt[52];
#endif

#endif // SM2_CORE_H