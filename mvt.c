#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"

// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif

void turn_angle(uint8_t sn, int angle, int ramp, int vit, FLAGS_T state, int motor)
{
  if ( ev3_search_tacho_plugged_in(motor,0, &sn, 0 )){
    int max_speed;
    get_tacho_max_speed( sn, &max_speed );
    set_tacho_stop_action_inx( sn, TACHO_COAST );
    set_tacho_speed_sp( sn, max_speed / vit );
    set_tacho_ramp_up_sp( sn, ramp );
    set_tacho_ramp_down_sp( sn, ramp );
    set_tacho_position_sp( sn, angle );
    set_tacho_command_inx( sn, TACHO_RUN_TO_REL_POS );
    do {
        get_tacho_state_flags( sn, &state );
      } while ( state);
    } else {
      printf( "LEGO_EV3_M_MOTOR 1 is NOT found\n" );
    }
    return;
}

void mvt_forward(uint8_t l_sn, uint8_t r_sn, int time, int ramp, int l_vit, int r_vit, FLAGS_T l_state, FLAGS_T r_state)
{
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

void mvt_turn_one_wheel(uint8_t sn, int time, int ramp, int vit, FLAGS_T state, int motor)
{
    if ( ev3_search_tacho_plugged_in(motor,0, &sn, 0 ) ){
      int max_speed;
      get_tacho_max_speed( sn, &max_speed );
      set_tacho_stop_action_inx( sn, TACHO_COAST );
      max_speed = max_speed;
      set_tacho_speed_sp(sn, max_speed * 1/vit );
      set_tacho_time_sp( sn, time );
      set_tacho_ramp_up_sp( sn, ramp );
      set_tacho_ramp_down_sp( sn, ramp );
      set_tacho_command_inx( sn, TACHO_RUN_TIMED );
      do {
        get_tacho_state_flags( sn, &state );
      } while ( state);
    } else {
      printf( "LEGO_EV3_M_MOTOR 1 is NOT found\n" );
    }
    return;
}


