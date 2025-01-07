#ifndef STEP1S_H
#define STEP1S_H

void mvt_motor(int time, int ramp, int l_vit, int r_vit);

float sonar();

float compas();

int couleur(int stp);

int touch(int time, int ramp, int l_vit, int r_vit);

void test_system();

void turn(int time, int ramp, int l_vit, int r_vit, int index, float degre) ;

void stay(int time, int ramp, int max_vit, int min_vit, float degre, float ecart);

#endif