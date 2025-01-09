#ifndef MVT_H
#define MVT_H


void turn_arm(int angle);

void mvt_forward(int time, int ramp, int l_vit, int r_vit);

float turn_precise(float final_angle, float tol);

void forward_to_wall(float dist_min, float start_angle, float tol_dev, float tol_angle);

#endif