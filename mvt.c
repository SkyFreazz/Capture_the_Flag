#include <stdio.h>
#include <stdlib.h>
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
      set_tacho_speed_sp( l_sn, l_max_speed * 1/l_vit );
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

      mvt_forward(100, 0, 3.01, 3);
    }
  }
}

