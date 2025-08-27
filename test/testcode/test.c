#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "sm2.h"
#include "sm3.h"
#include "sm4.h"

int test_fp_add(void) {
    sm2_fp x, y, t, r;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");
    sm2_fp_from_str(y, "9DDD52AF95B748A553D1B1E106627F901CD453F067A0D50202C672130C90F607");
    sm2_fp_from_str(r, "85978FF2D688C24F8C9732E0FFCB6DBEF81EE2E83705B80B4E3D7C1FD32116F5");
    sm2_fp_add(t, x, y);
    return sm2_fp_equ(t, r);
}

int test_fp_sub(void) {
    sm2_fp x, y, t, r;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");
    sm2_fp_from_str(y, "FDDD52AF95B748A553D1B1E106627F901CD453F067A0D50202C672130C90F607");
    sm2_fp_from_str(r, "E9DCEA91AB1A3104E4F3CF1EF3066E9EBE763B0567C40E0948B097F9B9FF2AE5");
    sm2_fp_sub(t, x, y);
    return sm2_fp_equ(t, r);
}

int test_fp_neg(void) {
    sm2_fp x, t, r;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");
    sm2_fp_from_str(r, "1845C2BCBF2E8655C73A7F00069711D124B57108309B1CF6B488F5F3396FDF12");
    sm2_fp_neg(t, x);
    return sm2_fp_equ(t, r);
}

int test_fp_dou(void) {
    sm2_fp x, t, r;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");
    sm2_fp_from_str(r, "CF747A8581A2F354718B01FFF2D1DC5DB6951DEE9EC9C61396EE14198D2041DB");
    sm2_fp_dbl(t, x);
    return sm2_fp_equ(t, r);
}

int test_fp_haf(void) {
    sm2_fp x1, x2, t1, t2, r1, r2;
    sm2_fp_from_str(x1, "9DDD52AF95B748A553D1B1E106627F901CD453F067A0D50202C672130C90F606");
    sm2_fp_from_str(x2, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");
    sm2_fp_from_str(r1, "4EEEA957CADBA452A9E8D8F083313FC80E6A29F833D06A810163390986487B03");
    sm2_fp_from_str(r2, "F3DD1EA0A068BCD51C62C07FFCB477176DA5477AE7B27185A5BB850663481076");
    sm2_fp_haf(t1, x1);
    sm2_fp_haf(t2, x2);
    return sm2_fp_equ(t1, r1) && sm2_fp_equ(t2, r2);
}

int test_fp_inv(void) {
    sm2_fp x, t, r;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");
    sm2_fp_from_str(r, "28E9FA9E9D9F5E344D5A9E4BCF6509A7F39789F515AB8F92DDBCBD414D940E93");
    sm2_fp_inv(t, x);
    return sm2_fp_equ(t, r);
}

int test_fp_mul(void) {
    sm2_fp x, y, t, r;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");
    sm2_fp_from_str(y, "9DDD52AF95B748A553D1B1E106627F901CD453F067A0D50202C672130C90F607");
    sm2_fp_from_str(r, "4084F63BFFAE5AA8701C27E1F3C197299E064097A4B6D1A5A28E90872A095614");
    sm2_fp_mul(t, x, y);
    return sm2_fp_equ(t, r);
}

int test_fp_sqr(void) {
    sm2_fp x, t, r;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");
    sm2_fp_from_str(r, "CC59F89406D2F4217BAAD8BCD1E143D8822D69A822371F25413983413A743069");
    sm2_fp_sqr(t, x);
    return sm2_fp_equ(t, r);
}

int test_fn_add(void) {
    sm2_fp x, y, t, r;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");
    sm2_fp_from_str(y, "9DDD52AF95B748A553D1B1E106627F901CD453F067A0D50202C672130C90F607");
    sm2_fp_from_str(r, "85978FF2D688C24F8C9732E0FFCB6DBF861B037C153FB2E0FA818816994BD5D1");
    sm2_fn_add(t, x, y);
    return sm2_fp_equ(t, r);
}

int test_fn_sub(void) {
    sm2_fp x, y, t, r;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");
    sm2_fp_from_str(y, "FDDD52AF95B748A553D1B1E106627F901CD453F067A0D50202C672130C90F607");
    sm2_fp_from_str(r, "E9DCEA91AB1A3104E4F3CF1EF3066E9E307A1A71898A13339C6C8C02F3D46C09");
    sm2_fn_sub(t, x, y);
    return sm2_fp_equ(t, r);
}

int test_fn_inv(void) {
    sm2_fp x, t, r;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");
    sm2_fp_from_str(r, "4E40BE003921C68372FFB6B2F75C5CC22E007A35FFF138434643A4524A727272");
    sm2_fn_inv(t, x);
    return sm2_fp_equ(t, r);
}

int test_fn_mul(void) {
    sm2_fp x, y, t, r;
    sm2_fp_from_str(x, "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED");
    sm2_fp_from_str(y, "9DDD52AF95B748A553D1B1E106627F901CD453F067A0D50202C672130C90F607");
    sm2_fp_from_str(r, "E2B6D58F9FC8CC4011850CD77E173F39E929392ED09404A67B842F4ADF2C02C9");
    sm2_fn_mul(t, x, y);
    return sm2_fp_equ(t, r);
}

int test_jpoint_add(void) {
    sm2_jpoint jp, jq, jt;
    sm2_apoint t, r;
    sm2_jpoint_from_str(&jp,    "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED"
                                "9DDD52AF95B748A553D1B1E106627F901CD453F067A0D50202C672130C90F607"
                                "0000000000000000000000000000000000000000000000000000000000000001");
    sm2_jpoint_from_str(&jq,    "421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D"
                                "0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2"
                                "0000000000000000000000000000000000000000000000000000000000000001");
    sm2_apoint_from_str(&r,     "68A499B8B89806D30852AF8D85D07FF70A30F93B1A416AC6A1C411E1E7D261F1"
                                "7EB38011FC432E66F5E6166CB9E461008A2CD82B5A14B4AFDB25DEC8B7622B4B");

    sm2_jpoint_add(&jt, &jp, &jq);
    sm2_jpoint_to_apoint(&jt, &t);
    return sm2_fp_equ(t.x, r.x) & sm2_fp_equ(t.y, r.y);
}

int test_japoint_add(void) {
    sm2_apoint q, r, t;
    sm2_jpoint jp, jt;
    sm2_jpoint_from_str(&jp,    "E7BA3D4240D179AA38C580FFF968EE2EDB4A8EF6CF64E30A4B770A0CC69020ED"
                                "9DDD52AF95B748A553D1B1E106627F901CD453F067A0D50202C672130C90F607"
                                "0000000000000000000000000000000000000000000000000000000000000001");
    sm2_apoint_from_str(&q,     "421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D"
                                "0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2");
    sm2_apoint_from_str(&r,     "68A499B8B89806D30852AF8D85D07FF70A30F93B1A416AC6A1C411E1E7D261F1"
                                "7EB38011FC432E66F5E6166CB9E461008A2CD82B5A14B4AFDB25DEC8B7622B4B");

    sm2_japoint_add(&jt, &jp, &q);
    sm2_jpoint_to_apoint(&jt, &t);
    return sm2_fp_equ(t.x, r.x) & sm2_fp_equ(t.y, r.y);
}

int test_jpoint_dou(void) {
    sm2_apoint r, t;
    sm2_jpoint jp, jt;
    sm2_jpoint_from_str(&jp,    "421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D"
                                "0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2"
                                "0000000000000000000000000000000000000000000000000000000000000001");
    sm2_apoint_from_str(&r,     "61C36A2DEF5DC5E56C4F9A77B16FC92AEC342D5EAE52521F716A38D3A46C84D3"
                                "A16FB5481138B6BDF81164EB5CC1721EA755DA1A240F9D19E941E2166516AD16");
    sm2_jpoint_dou(&jt, &jp);
    sm2_jpoint_to_apoint(&jt, &t);
    return sm2_fp_equ(t.x, r.x) & sm2_fp_equ(t.y, r.y);
}

int test_jpoint_mdo(void) {
    sm2_apoint r, t;
    sm2_jpoint jp, jt;
    u32 m = 6;
    sm2_jpoint_from_str(&jp,    "421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D"
                                "0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2"
                                "0000000000000000000000000000000000000000000000000000000000000001");
    sm2_apoint_from_str(&r,     "A2BCEBCC908C39CC2091090F0375D1271968790DC2914F348EE76CFC6E3AF8C1"
                                "65F2C7D3A7D1D516E60B476A71C0E62BA62F0939CC6BA0134358E54BB57FFD93");
    sm2_jpoint_mdo(&jt, m, &jp);
    sm2_jpoint_to_apoint(&jt, &t);
    return sm2_fp_equ(t.x, r.x) & sm2_fp_equ(t.y, r.y);
}

int test_point_mul(void) {
    sm2_fp k;
    sm2_point p, t, r;
    sm2_fp_from_str(k,      "3945208F7B2144B13F36E38AC6D39F95889393692860B51A42FB81EF4DF7C5B8");
    sm2_point_from_str(&p,  "04EBFC718E8D1798620432268E77FEB6415E2EDE0E073C0F4F640ECD2E149A73"
                            "E858F9D81E5430A57B36DAAB8F950A3C64E6EE6A63094D99283AFF767E124DF0");
    sm2_point_from_str(&r,  "335E18D751E51F040E27D468138B7AB1DC86AD7F981D7D416222FD6AB3ED230D"
                            "AB743EBCFB22D64F7B6AB791F70658F25B48FA93E54064FDBFBED3F0BD847AC9");
    sm2_point_mul(&t, k, &p);
    return memcmp(t.x, r.x, 32) == 0 && memcmp(t.x, r.x, 32) == 0;
}

int test_point_gen(void) {
    sm2_fp k;
    sm2_point r, t;
    sm2_fp_from_str(k,      "3945208F7B2144B13F36E38AC6D39F95889393692860B51A42FB81EF4DF7C5B8");
    sm2_point_from_str(&r,  "09F9DF311E5421A150DD7D161E4BC5C672179FAD1833FC076BB08FF356F35020"
                            "CCEA490CE26775A52DC6EA718CC1AA600AED05FBF35E084A6632F6072DA9AD13");
    sm2_point_gen(&t, k);
    return memcmp(t.x, r.x, 32) == 0 && memcmp(t.x, r.x, 32) == 0;
}

int test_sm3(void) {
    // Test Example 1 of GB/T 32905-2016
    const char message1[] = "abc";
    u8 digest1[32];
    u8 rdigest1[32];
    bytes_from_hexstr(rdigest1, "66C7F0F462EEEDD9D1F2D46BDC10E4E24167C4875CF2F7A2297DA02B8F4BA8E0", 32);
    sm3(digest1, message1, strlen(message1));


    // Test Example 2 of GB/T 32905-2016
    const char message2[] = "abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd";
    u8 digest2[32];
    u8 rdigest2[32];
    bytes_from_hexstr(rdigest2, "DEBE9FF92275B8A138604889C18E5A4D6FDB70E5387E5765293DCBA39C0C5732", 32);
    sm3(digest2, message2, strlen(message2));
    return memcmp(digest1, rdigest1, 32) == 0 && memcmp(digest2, rdigest2, 32) == 0;
}

int test_sm2_sign_and_verify(void) {

    const char id[] = "1234567812345678";
    const char msg[] = "message digest";
    const char key[] = "3945208F7B2144B13F36E38AC6D39F95889393692860B51A42FB81EF4DF7C5B8";
    const char k[] = "59276E27D506861A16680F3AD9C02DCCEF3CC1FA3CDBE4CE6D54B80DEAC1BC21";

    sm2_prikey prikey;
    rand_pad(key, 32);
    sm2_keygen(&prikey);

    u8 sig0[72], sig1[72];
    u64 slen0 = 72, slen1 = 0;
    bytes_from_hexstr(sig0, "3046022100f5a03b0648d2c4630eeac513e1bb81a15944da3827d5b74143ac7eaceee720b3022100b1b6aa29df212fd8763182bc0d421ca1bb9038fd1f7f42d4840b69c485bbc1aa", 72);

    // test sign
    rand_pad(k, 32);
    sm2_sign(&prikey, sig1, &slen1, id, strlen(id), msg, strlen(msg));
    if ((slen0 != slen1) || memcmp(sig0, sig1, sizeof(sig1)) != 0) {
        goto error;
    }

    sm2_sign_ctx sctx;
    rand_pad(k, 32);
    sm2_sign_init(&sctx, &prikey, id, strlen(id));
    sm2_sign_update(&sctx, msg, strlen(msg));
    sm2_sign_finish(&sctx, sig1, &slen1);

    if ((slen0 != slen1) || memcmp(sig0, sig1, sizeof(sig1)) != 0) {
        goto error;
    }

    // test verify
    if (sm2_verify(&prikey.pubkey, sig0, sizeof(sig0), id, strlen(id), msg, strlen(msg)) != 1) {
        goto error;
    }

    sm2_verify_ctx vctx;
    sm2_verify_init(&vctx, &prikey.pubkey, id, strlen(id));
    sm2_verify_update(&vctx, msg, strlen(msg));
    if (sm2_verify_finish(&vctx, sig0, sizeof(sig0)) != 1) {
        goto error;
    }

    // test sign and verify multiple times ture random number k
    for (int i = 0; i < 16; i++) {
        sm2_sign(&prikey, sig1, &slen1, id, strlen(id), msg, strlen(msg));
        if (sm2_verify(&prikey.pubkey, sig1, slen1, id, strlen(id), msg, strlen(msg)) != 1) {
            goto error;
        }
    }

    return 1;
    error:
    return 0;
}

int test_sm2_enc_and_dec(void) {

    char msg[] = "encryption standard";
    const char key[] = "3945208F7B2144B13F36E38AC6D39F95889393692860B51A42FB81EF4DF7C5B8";
    const char k[] = "59276E27D506861A16680F3AD9C02DCCEF3CC1FA3CDBE4CE6D54B80DEAC1BC21";

    sm2_prikey prikey;
    rand_pad(key, 32);
    sm2_keygen(&prikey);

    u64 cl0 = 126, cl1 = 0, pl0 = strlen(msg), pl1 = 0;
    u8 ct0[126], ct1[150], pt[150];
    bytes_from_hexstr(ct0, "307c022004ebfc718e8d1798620432268e77feb6415e2ede0e073c0f4f640ecd"
                           "2e149a73022100e858f9d81e5430a57b36daab8f950a3c64e6ee6a63094d9928"
                           "3aff767e124df0042059983c18f809e262923c53aec295d30383b54e39d609d1"
                           "60afcb1908d0bd8766041321886ca989ca9c7d58087307ca93092d651efa", 126);

    // encryption
    rand_pad(k, 32);
    sm2_encrypt(&prikey.pubkey, ct1, &cl1, msg, pl0);
    if (cl1 != cl0 || memcmp(ct1, ct0, sizeof(ct0)) != 0)
        goto error;

    // decryption
    sm2_decrypt(&prikey, pt, &pl1, ct0, cl0);
    if (pl1 != pl0 || memcmp(pt, msg, pl0) != 0)
        goto error;

    // test encryption and decryption multiple times with ture random number k
    for (int i = 0; i < 16; i++) {
        sm2_encrypt(&prikey.pubkey, ct1, &cl1, msg, pl0);
        sm2_decrypt(&prikey, pt, &pl1, ct0, cl0);
        if (pl1 != pl0 || memcmp(pt, msg, pl0) != 0)
            goto error;
    }

    return 1;
    error:
    return 0;
}

int test_sm4_enc_and_dec(void) {

    sm4_ctx *ctx = malloc(sizeof(sm4_ctx));

    // Test Example 1 of GB/T 32907-2016
    const u8 kt1[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
    const u8 pt1[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
    const u8 ct1[16] = {0x68, 0x1e, 0xdf, 0x34, 0xd2, 0x06, 0x96, 0x5e, 0x86, 0xb3, 0xe9, 0x4f, 0x53, 0x6e, 0x42, 0x46};
    u8 rt1[16] = {0};
    sm4_setkey(ctx, kt1, sizeof(kt1));
    sm4_encrypt(ctx, pt1, rt1, sizeof(pt1));
    if (memcmp(rt1, ct1, 16) != 0)
        goto error;
    sm4_decrypt(ctx, ct1, rt1, sizeof(ct1));
    if (memcmp(rt1, pt1, 16) != 0)
        goto error;

    // Test Example 2 of GB/T 32907-2016
    const u8 kt2[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
    const u8 pt2[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x98, 0x76, 0x54, 0x32, 0x10};
    const u8 ct2[16] = {0x59, 0x52, 0x98, 0xC7, 0xC6, 0xFD, 0x27, 0x1F, 0x04, 0x02, 0xF8, 0x04, 0xC3, 0x3D, 0x3F, 0x66};
    u8 rt2[16] = {0};
    sm4_setkey(ctx, kt2, sizeof(kt2));
    memcpy(rt2, pt2, sizeof(pt2));
    for (int i = 1000000; i > 0; i--)
        sm4_encrypt(ctx, rt2, rt2, sizeof(rt2));
    if (memcmp(rt2, ct2, 16) != 0)
        goto error;

    memcpy(rt2, ct2, sizeof(ct2));
    for (int i = 1000000; i > 0; i--)
        sm4_decrypt(ctx, rt2, rt2, sizeof(rt2));
    if (memcmp(rt2, pt2, 16) != 0)
        goto error;

    free(ctx);
    return 1;
    error:
    free(ctx);
    return 0;
}

void test_all(void) {

    int result;
    const char* result_string[] = { "Error", "Right" };

    printf("Start testing each module for correctness...\n");
    printf("Module\t\t\tResult\n");
    // test fp
    result = 1;
    result &= test_fp_add();
    result &= test_fp_sub();
    result &= test_fp_neg();
    result &= test_fp_haf();
    result &= test_fp_dou();
    result &= test_fp_mul();
    result &= test_fp_sqr();
    result &= test_fp_inv();
    printf("%-16s%s\n", "FP", result_string[result]);

    // test fn
    result = 1;
    result &= test_fn_add();
    result &= test_fn_sub();
    result &= test_fn_mul();
    result &= test_fn_inv();
    printf("%-16s%s\n", "FN", result_string[result]);

    // test point
    result = 1;
    result &= test_jpoint_add();
    result &= test_japoint_add();
    result &= test_jpoint_dou();
    result &= test_jpoint_mdo();
    result &= test_point_mul();
    result &= test_point_gen();
    printf("%-16s%s\n", "POINT", result_string[result]);

    // test sm2
    result = 1;
    result &= test_sm2_sign_and_verify();
    result &= test_sm2_enc_and_dec();

    printf("%-16s%s\n", "SM2", result_string[result]);
    //

    // test sm3
    result = 1;
    result &= test_sm3();
    printf("%-16s%s\n", "SM3", result_string[result]);

    // test sm4
    result = 1;
    result &= test_sm4_enc_and_dec();
    printf("%-16s%s\n", "SM4", result_string[result]);
    printf("The test is over.\n\n");

}
int main(void) {

    test_all();

}