#ifndef STEP1_H
#define STEP1_H

void mvt_motor(int time, int ramp, int l_vit, int r_vit);

float compas();

float sonar();

int couleur(int stp);

int touch(int time, int ramp, int l_vit, int r_vit);

void test_system();

void turn(int time, int ramp, int l_vit, int r_vit, int index, float degre);

#endif