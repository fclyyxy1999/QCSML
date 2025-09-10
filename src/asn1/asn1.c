#include "asn1.h"

int asn1_length_to_der(const u64 len, u8 **dst, u64 *dstlen) {
    if (len > 0x7fffffff)
        return 0;
    if (len < 128) {
        *((*dst)++) = (u8) len;
        (*dstlen)++;
    } else {
        u8 buf[4];
        int nbytes;
        if (len < 256) nbytes = 1;
        else if (len < 65536) nbytes = 2;
        else if (len < (1 << 24)) nbytes = 3;
        else nbytes = 4;
        PUT32(buf, (u32) len);
        *((*dst)++) = 0x80 + nbytes;
        memcpy(*dst, buf + 4 - nbytes, nbytes);
        *dst += nbytes;
        *dstlen += nbytes + 1;
    }
    return 1;
}

int asn1_length_from_der(u64 *len, const u8 **src, u64 *srclen) {
    if (**src < 128) {
        (*len) = *((*src)++);
        (*srclen)--;
    } else {
        u8 buf[4] = {0};
        int nbytes = *((*src)++) & 0x7f;
        (*srclen)--;
        memcpy(buf + 4 - nbytes, *src, nbytes);
        *len = GET32(buf);
        *src += nbytes;
        *srclen -= nbytes;
    }
    return 1;
}

int asn1_boolean_to_der(const int val, u8 **dst, u64 *dstlen) {
    if (val < 0)
        return 0;
    *((*dst)++) = TAG_BOOLEAN;
    *((*dst)++) = 1;
    *((*dst)++) = val ? 0xff : 0x00;
    *dstlen += 3;
    return 1;
}

int asn1_boolean_from_der(int *val, const u8 **src, u64 *srclen) {
    if (*((*src)++) != TAG_BOOLEAN || *((*src)++) != 1)
        return 0;
    *val = *((*src)++) != 0;
    *srclen -= 3;
    return 1;
}

int asn1_integer_to_der(const u8 *num, u64 numlen, u8 **dst, u64 *dstlen) {
    // tag
    *((*dst)++) = TAG_INTEGER;
    (*dstlen)++;

    // Remove the first consecutive 0 from the number, ensuring that the first byte is not 0
    while (*num == 0 && numlen > 1) {
        num++;
        numlen--;
    }
    if (num[0] >> 7) {
        asn1_length_to_der(numlen + 1, dst, dstlen);
        *((*dst)++) = 0x00;
        memcpy(*dst, num, numlen);
        *dst += numlen;
        *dstlen += numlen + 1;
    } else {
        asn1_length_to_der(numlen, dst, dstlen);
        memcpy(*dst, num, numlen);
        *dst += numlen;
        *dstlen += numlen;
    }
    return 1;
}

int asn1_integer_from_der(u8 *num, u64 *numlen, const u8 **src, u64 *srclen) {
    if ((*srclen)-- == 0 || *((*src)++) != TAG_INTEGER)
        return 0;
    asn1_length_from_der(numlen, src, srclen);
    // remove leading zero
    if ((**src == 0) && (*numlen > 1)) {
        (*src)++;
        (*srclen)--;
        (*numlen)--;
    }
    memcpy(num, *src, *numlen);
    *src += *numlen;
    *srclen -= *numlen;
    return 1;
}

int asn1_bit_string_to_der(const u8 *bits, const u64 nbits, u8 **dst, u64 *dstlen) {
    // tag
    *((*dst)++) = TAG_BIT_STRING;
    (*dstlen)++;

    // length
    u64 nbytes = (nbits + 7) >> 3;
    u64 unused_nbits = (nbytes << 3) - nbits;
    asn1_length_to_der(nbytes + 1, dst, dstlen);
    // unused num of bits
    *((*dst)++) = (u8) unused_nbits;
    (*dstlen)++;

    // bits
    memcpy(*dst, bits, nbytes);
    *dst += nbytes;
    *dstlen += nbytes;
    return 1;
}

int asn1_bit_string_from_der(u8 *bits, u64 *nbits, const u8 **src, u64 *srclen) {
    if ((*srclen)-- == 0 || *((*src)++) != TAG_BIT_STRING)
        return 0;

    u64 len = 0;
    int unused_bits;
    asn1_length_from_der(&len, src, srclen);

    // unused_bits counter
    unused_bits = *((*src)++);
    (*srclen)--;
    len--;

    // return bits
    memcpy(bits, *src, len);
    *nbits = (len << 3) - unused_bits;
    *src += len;
    *srclen -= len;
    return 1;
}

int asn1_octets_to_der(const u8 *octets, const u64 noctets, u8 **dst, u64 *dstlen) {
    // tag
    *((*dst)++) = TAG_OCTET_STRING;
    (*dstlen)++;

    // length
    asn1_length_to_der(noctets, dst, dstlen);

    // octets
    memcpy(*dst, octets, noctets);
    *dst += noctets;
    *dstlen += noctets;
    return 1;
}

int asn1_octets_from_der(u8 *octets, u64 *noctets, const u8 **src, u64 *srclen) {
    if ((*srclen)-- == 0 || *((*src)++) != TAG_OCTET_STRING)
        return 0;

    // length
    asn1_length_from_der(noctets, src, srclen);

    // return
    memcpy(octets, *src, *noctets);
    *src += *noctets;
    *srclen -= *noctets;
    return 1;
}

int asn1_sequence_to_der(const u8 *data, const u64 datalen, u8 **dst, u64 *dstlen) {
    // tag
    *((*dst)++) = TAG_SEQUENCE;
    (*dstlen)++;

    // length
    asn1_length_to_der(datalen, dst, dstlen);

    // data
    memcpy(*dst, data, datalen);
    *dst += datalen;
    *dstlen += datalen;
    return 1;
}

int asn1_sequence_from_der(u8 *data, u64 *datalen, const u8 **src, u64 *srclen) {
    if ((*srclen)-- == 0 || *((*src)++) != TAG_SEQUENCE)
        return 0;

    asn1_length_from_der(datalen, src, srclen);

    // return
    memcpy(data, *src, *datalen);
    *src += *datalen;
    *srclen -= *datalen;
    return 1;
}

