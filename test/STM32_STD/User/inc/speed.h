#ifndef SM_SPEED_H
#define SM_SPEED_H



void speed_all(void);

void speed_fp_add(void);
void speed_fp_sub(void);
void speed_fp_haf(void);
void speed_fp_dou(void);
void speed_fp_inv(void);
void speed_fp_mul(void);

void speed_point_add(void);
void speed_point_dou(void);
void speed_point_mdo(void);
void speed_point_mul(void);
void speed_point_gen(void);

void speed_sm2_sign(void);
void speed_sm2_verify(void);
void speed_sm2_enc(void);
void speed_sm2_dec(void);

void speed_sm4_enc(void);
void speed_sm4_dec(void);

void speed_sm3(void);

#endif //SM_SPEED_H
