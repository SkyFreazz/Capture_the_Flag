#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"

#define R_WHEEL 65
#define L_WHEEL 68
#define ARM 67

// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif

const char const *color[] = { "?", "BLACK", "BLUE", "GREEN", "YELLOW", "RED", "WHITE", "BROWN" };
#define COLOR_COUNT  (( int )( sizeof( color ) / sizeof( color[ 0 ])))

static bool _check_pressed( uint8_t sn )
{
  int val;

  if ( sn == SENSOR__NONE_ ) {
    return ( ev3_read_keys(( uint8_t *) &val ) && ( val & EV3_KEY_UP ));
  }
  return ( get_sensor_value( 0, sn, &val ) && ( val != 0 ));
}

void turn_arm(int vit, int time)
{
  uint8_t sn;
  FLAGS_T state;

  if ( ev3_search_tacho_plugged_in(ARM,0, &sn, 0 )){
      int max_speed;
      get_tacho_max_speed( sn, &max_speed );
      set_tacho_stop_action_inx( sn, TACHO_COAST );
      max_speed = -max_speed;
      set_tacho_speed_sp( sn, max_speed * 1/vit );
      set_tacho_time_sp( sn, time );
      set_tacho_ramp_up_sp( sn, time/3 );
      set_tacho_ramp_down_sp( sn, time/3 );
      set_tacho_command_inx( sn, TACHO_RUN_TIMED );
      do {
        get_tacho_state_flags( sn, &state );
      } while ( state );
    } else {
      printf( "LEGO_EV3_M_MOTOR 1 is NOT found\n" );
    }
    return;

  return;
}

void mvt_forward(int time, int ramp, int l_vit, int r_vit)
{
  uint8_t l_sn, r_sn;
  FLAGS_T l_state, r_state;

    if ( ev3_search_tacho_plugged_in(L_WHEEL,0, &l_sn, 0 ) && ev3_search_tacho_plugged_in(R_WHEEL,0, &r_sn, 0 ) ){
      int l_max_speed;
      int r_max_speed;
      get_tacho_max_speed( l_sn, &l_max_speed );
      get_tacho_max_speed( r_sn, &r_max_speed );
      set_tacho_stop_action_inx( l_sn, TACHO_COAST );
      set_tacho_stop_action_inx( r_sn, TACHO_COAST );
      l_max_speed = -l_max_speed;
      r_max_speed = -r_max_speed;
      set_tacho_speed_sp( l_sn, (l_max_speed * 1/l_vit) + 0.1 );
      set_tacho_speed_sp( r_sn, r_max_speed * 1/r_vit );
      set_tacho_time_sp( l_sn, time );
      set_tacho_time_sp( r_sn, time );
      set_tacho_ramp_up_sp( l_sn, ramp );
      set_tacho_ramp_up_sp( r_sn, ramp );
      set_tacho_ramp_down_sp( l_sn, ramp );
      set_tacho_ramp_down_sp( r_sn, ramp );
      set_tacho_command_inx( l_sn, TACHO_RUN_TIMED );
      set_tacho_command_inx( r_sn, TACHO_RUN_TIMED );
      do {
        get_tacho_state_flags( l_sn, &l_state );
        get_tacho_state_flags( r_sn, &r_state );
      } while ( l_state && r_state );
    } else {
      printf( "LEGO_EV3_M_MOTOR 1 is NOT found\n" );
    }
    return;
}

void turn_precise(float final_angle, float tol){

  uint8_t sn_compass;
  float angle;

  if(ev3_search_sensor(LEGO_EV3_GYRO, &sn_compass,0)){
    while(true){
      if ( !get_sensor_value0(sn_compass, &angle )) {
        angle = 0;
      }

      angle -= final_angle;

      if (fabs(fmod(angle, 360.0)) <= tol){
        return;
      }

      float angle_180 = roundf(fmod(fmod(angle, 360.0) + 360.0, 360.0));

      if (angle_180 >= 180){
        mvt_forward(100, 35, 6, -6);
      } else {
        mvt_forward(100, 35, -6, 6);
      }
    }
  }
  return;
}

void forward_to_wall(float dist_min, float start_angle, float tol_dev, float tol_angle){

  uint8_t sn_sonar, sn_compass, sn_touch;
  float value, angle;

  ev3_search_sensor( LEGO_EV3_TOUCH, &sn_touch, 0 );

  if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0)){
    while(true){
      if (!get_sensor_value0(sn_sonar, &value )) {
        value = 0;
      }

      if (value <= dist_min && _check_pressed( sn_touch )){
        return;
      }

      if(ev3_search_sensor(LEGO_EV3_GYRO, &sn_compass,0)){
        if ( !get_sensor_value0(sn_compass, &angle )) {
          angle = 0;
        }

        if (fabs(start_angle - angle) >= tol_dev){
          turn_precise(start_angle, tol_angle);
        }
      }

      mvt_forward(100, 0, 3, 3);
    }
  }
}

void forward_to_base(float dist_min, float initial_angle, float first_angle, float second_angle, float tol_dev, float tol_angle){
  uint8_t sn_sonar, sn_compass, sn_touch, sn_color;
  int val;
  float value, angle;
  float current_angle = first_angle;
  int phase = 1;
  int cross_2 = 0;

  ev3_search_sensor( LEGO_EV3_TOUCH, &sn_touch, 0 );

  if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0)){
    while(phase != 4){

      printf("phase: %d\n", phase);

      //get all the values

      //angle + adjustment
      if(ev3_search_sensor(LEGO_EV3_GYRO, &sn_compass,0)){
        if ( !get_sensor_value0(sn_compass, &angle )) {
          angle = 0;
        }

        if (fabs(current_angle - angle) >= tol_dev){
          turn_precise(current_angle, tol_angle);
        }
      }

      //distance
      if (!get_sensor_value0(sn_sonar, &value )) { 
        value = 0;
      }
      if ( (value < 40.0) || (value > 2500.0) ){
        value = 0;
      }

      //color
      if ( ev3_search_sensor( LEGO_EV3_COLOR, &sn_color, 0 )) {
        if ( !get_sensor_value( 0, sn_color, &val ) || ( val < 0 ) || ( val >= COLOR_COUNT )) {
          val = 0;
        }
        if (strcmp(color[val], "BLACK") == 0){
          printf("middle line detect\n");
          phase = 2;
        }else if (strcmp(color[val], "GREEN") == 0 || strcmp(color[val], "YELLOW") == 0) {
          if (phase == 2 ) {
              printf("green line detected\n");
              phase = 3;               
          }
        }
      }

      if (value <= dist_min){ //there's an object
        printf("OBJECT\n");
        if (phase == 1){
          mvt_forward(500, 100, -2, -2);
          current_angle = (initial_angle + (initial_angle  - first_angle));
          turn_precise(current_angle, 1);
        }

        if(phase == 2){
          mvt_forward(200, 50, -2, -2);
          if(first_angle > initial_angle){
            turn_precise(initial_angle - 90.0, 1);
          } else {
            turn_precise(initial_angle + 90.0, 1);
          }
          mvt_forward(1000, 250, 2, 2);
          turn_precise(second_angle, 1);
        }
      }

      if ((phase == 2) && (cross_2 <= 1)){ //we are in the middle
        if(cross_2 == 1){
          turn_precise(second_angle, 1);
          current_angle = second_angle;
          cross_2 +=1;
        } else {
          mvt_forward(300, 100, 1, 1);
          cross_2 +=1;
        }
      }

      if (phase == 3){
        turn_precise(initial_angle, 1);
        current_angle = initial_angle;
      }

      mvt_forward(100, 0, 4, 4);
    }
  }
  return;
}