#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"

#define R_WHEEL 65
#define L_WHEEL 68
#define ARM 67
#define M_SENSOR 66
// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif

void mvt_motor(uint8_t l_sn, uint8_t r_sn, int time, int ramp, int vit, FLAGS_T l_state, FLAGS_T r_state)
{
    if ( ev3_search_tacho_plugged_in(L_WHEEL,0, &l_sn, 0 ) && ev3_search_tacho_plugged_in(R_WHEEL,0, &r_sn, 0 ) ){
      int l_max_speed;
      int r_max_speed;
      get_tacho_max_speed( l_sn, &l_max_speed );
      printf("%u\n", l_sn);
      get_tacho_max_speed( r_sn, &r_max_speed );
      set_tacho_stop_action_inx( l_sn, TACHO_COAST );
      printf("%u\n", l_sn);
      set_tacho_stop_action_inx( r_sn, TACHO_COAST );
      l_max_speed = -l_max_speed;
      r_max_speed = -r_max_speed;
      set_tacho_speed_sp( l_sn, l_max_speed * vit );
      printf("%u\n", l_sn);
      set_tacho_speed_sp( r_sn, r_max_speed * vit );
      set_tacho_time_sp( l_sn, time );
      printf("%u\n", l_sn);
      set_tacho_time_sp( r_sn, time );
      set_tacho_ramp_up_sp( l_sn, ramp );
      printf("%u\n", l_sn);
      set_tacho_ramp_up_sp( r_sn, ramp );
      set_tacho_ramp_down_sp( l_sn, ramp );
      printf("%u\n", l_sn);
      set_tacho_ramp_down_sp( r_sn, ramp );
      set_tacho_command_inx( l_sn, TACHO_RUN_TIMED );
      printf("%u\n", l_sn);
      set_tacho_command_inx( r_sn, TACHO_RUN_TIMED );
      do {
        get_tacho_state_flags( l_sn, &l_state );
        get_tacho_state_flags( r_sn, &r_state );
      } while ( l_state && r_state );
    } else {
      printf( "LEGO_EV3_M_MOTOR 1 is NOT found\n" );
    }
}

void mvt_l_motor(uint8_t l_sn, int time, int ramp, int vit, FLAGS_T l_state)
{
    if ( ev3_search_tacho_plugged_in(L_WHEEL,0, &l_sn, 0 ) ){
      int l_max_speed;
      get_tacho_max_speed( l_sn, &l_max_speed );
      set_tacho_stop_action_inx( l_sn, TACHO_COAST );
      l_max_speed = -l_max_speed;
      set_tacho_speed_sp( l_sn, l_max_speed * vit );
      set_tacho_time_sp( l_sn, time );
      set_tacho_ramp_up_sp( l_sn, ramp );
      set_tacho_ramp_down_sp( l_sn, ramp );
      set_tacho_command_inx( l_sn, TACHO_RUN_TIMED );
      do {
        get_tacho_state_flags( l_sn, &l_state );
      } while ( l_state);
    } else {
      printf( "LEGO_EV3_M_MOTOR 1 is NOT found\n" );
    }
}

void mvt_r_motor(uint8_t r_sn, int time, int ramp, int vit, FLAGS_T r_state)
{
    if (ev3_search_tacho_plugged_in(R_WHEEL,0, &r_sn, 0 ) ){
      int r_max_speed;
      get_tacho_max_speed( r_sn, &r_max_speed );
      set_tacho_stop_action_inx( r_sn, TACHO_COAST );
      r_max_speed = -r_max_speed;
      set_tacho_speed_sp( r_sn, r_max_speed * vit );
      set_tacho_time_sp( r_sn, time );
      set_tacho_ramp_up_sp( r_sn, ramp );
      set_tacho_ramp_down_sp( r_sn, ramp );
      set_tacho_command_inx( r_sn, TACHO_RUN_TIMED );
      do {
        get_tacho_state_flags( r_sn, &r_state );
      } while ( r_state );
    } else {
      printf( "LEGO_EV3_M_MOTOR 1 is NOT found\n" );
    }
}

int main( void )
{
  int i;

  FLAGS_T state;
  uint8_t sn_touch;
  uint8_t sn_color;
  uint8_t sn_compass;
  uint8_t sn_sonar;
  char s[ 256 ];
  int val;
  float value;
  uint32_t n, ii;
  int l_max_speed;
  uint8_t l_sn;
  int r_max_speed;
  uint8_t r_sn;
  int a_max_speed;
  uint8_t a_sn;
  int sm_max_speed;
  uint8_t sm_sn;

#ifndef __ARM_ARCH_4T__
  /* Disable auto-detection of the brick (you have to set the correct address below) */
  ev3_brick_addr = "192.168.0.204";

#endif
  if ( ev3_init() == -1 ) return ( 1 );

#ifndef __ARM_ARCH_4T__
  printf( "The EV3 brick auto-detection is DISABLED,\nwaiting %s online with plugged tacho...\n", ev3_brick_addr );

#else
  printf( "Waiting tacho is plugged...\n" );

#endif
  while ( ev3_tacho_init() < 1 ) Sleep( 1000 );

  printf( "*** ( EV3 ) Hello! ***\n" );

  printf( "Found tacho motors:\n" );
  
mvt_motor(l_sn, r_sn, 5000, 2000, 2/3);
printf("ici 1");
Sleep( 3000 );
mvt_l_motor(l_sn, 3000,  1000,  1/3);
printf("ici 2");
Sleep( 3000 );
mvt_motor(l_sn, r_sn, 5000, 500, 1);
printf("ici 3");
Sleep( 3000 );
mvt_r_motor(r_sn,  3000,  1000,  1/3);
return ( 0 );
}