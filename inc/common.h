#ifndef QCSML_COMMON_H
#define QCSML_COMMON_H
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// Type definitions for various integer types
typedef int8_t      i8;     /// 8-bit  signed integer
typedef int32_t     i32;    /// 32-bit signed integer
typedef int64_t     i64;    /// 64-bit signed integer
typedef uint8_t     u8;     /// 8-bit  unsigned integer
typedef uint32_t    u32;    /// 32-bit unsigned integer
typedef uint64_t    u64;    /// 64-bit unsigned integer

// Define _asmweak, it will be overwritten by the assembly implementation (if it exists).
#define _asmweak __attribute__((weak))

#define GET32(m)        ((u32)(m)[0] << 24 | (u32)(m)[1] << 16 | (u32)(m)[2] <<  8 | (u32)(m)[3])
#define PUT32(ptr, a)   ((ptr)[0] = ((a) >> 24), (ptr)[1] = ((a) >> 16), (ptr)[2] = ((a) >>  8), (ptr)[3] = (a))

/** * @brief Converts a hexadecimal string to a byte array.
 * @param byte Pointer to the byte array where the result will be stored.
 * @param hex Pointer to the hexadecimal string.
 * @param nbytes Number of bytes to convert.
 */
void bytes_from_str(void *byte, const char* hex, u32 nbytes);

/** * @brief Converts a byte array to a hexadecimal string.
 * @param byte Pointer to the byte array to convert.
 * @param hex Pointer to the string buffer where the result will be stored.
 * @param nbytes Number of bytes to convert.
 */
void bytes_to_str(const void *byte, char* hex, u32 nbytes);
#endif //QCSML_COMMON_H
