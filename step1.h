#ifndef STEP1_H
#define STEP1_H

void mvt_motor(uint8_t l_sn, uint8_t r_sn, int time, int ramp, int l_vit, int r_vit, FLAGS_T l_state, FLAGS_T r_state);

float sonar(uint8_t sn_sonar, float value);

float compas(uint8_t sn_compass, float value);

int color(uint8_t sn_color, int val, int stp);

int touch(uint8_t sn_compass, uint8_t sn_touch, uint8_t l_sn, uint8_t r_sn, int time, int ramp, int l_vit, int r_vit, FLAGS_T l_state, FLAGS_T r_state);

void test_system(uint8_t sn_sonar, uint8_t sn_compass, uint8_t sn_color, uint8_t sn_touch, uint8_t sn);

void turn(uint8_t sn_compass, uint8_t l_sn, uint8_t r_sn, int time, int ramp, int l_vit, int r_vit, FLAGS_T l_state, FLAGS_T r_state, int index, float degre);

#endif