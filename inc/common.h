#ifndef SM_COMMON_H
#define SM_COMMON_H
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// Type definitions for various integer types
typedef int8_t      i8;     /// 8-bit signed integer
typedef int32_t     i32;    /// 32-bit signed integer
typedef int64_t     i64;    /// 64-bit signed integer
typedef uint8_t     u8;     /// 8-bit unsigned integer
typedef uint32_t    u32;    /// 32-bit unsigned integer
typedef uint64_t    u64;    /// 64-bit unsigned integer

#define GET32(m)        ((u32)(m)[0] << 24 | (u32)(m)[1] << 16 | (u32)(m)[2] <<  8 | (u32)(m)[3])
#define PUT32(ptr, a)   ((ptr)[0] = ((a) >> 24), (ptr)[1] = ((a) >> 16), (ptr)[2] = ((a) >>  8), (ptr)[3] = (a))

void bytes_from_hexstr(void *b, const char* s, u32 nbytes);

#endif //SM_COMMON_H
