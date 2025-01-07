#ifndef MVT_H
#define MVT_H


void turn_angle(uint8_t sn, int angle, int ramp, int vit, FLAGS_T state, int motor);

void mvt_forward(uint8_t l_sn, uint8_t r_sn, int time, int ramp, int l_vit, int r_vit, FLAGS_T l_state, FLAGS_T r_state);

void mvt_turn_one_wheel(uint8_t sn, int time, int ramp, int vit, FLAGS_T state, int motor);

#endif