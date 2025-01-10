#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "mvt.h"
#include "flag.h"

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

int main( void )
{

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
  while ( ev3_tacho_init() < 1) Sleep( 1000 );

  printf( "*** ( EV3 ) Hello! ***\n" );

  printf( "Found tacho motors:\n" );


uint8_t sn_compass;

ev3_sensor_init();
float initial_angle;

if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_compass,0)){
  if ( !get_sensor_value0(sn_compass, &initial_angle )) {
    initial_angle = 0;
  }
  printf("initial_angle: %f", initial_angle);
}

//mvt_forward(4000, 0, 2, 2);


catch_flag(initial_angle);

 /* 
mvt_motor(l_sn, r_sn, 5000, 2000, 2, l_state, r_state);
printf("ici 1");
Sleep( 1000 );
mvt_l_motor(l_sn, 3000,  1000,  3, l_state);
printf("ici 2");
Sleep( 1000 );
mvt_motor(l_sn, r_sn, 5000, 500, 1, l_state, r_state);
printf("ici 3");
Sleep( 1000 );
mvt_r_motor(r_sn,  3000,  1000,  3, r_state);
Sleep(3000); 
turn_arm(a_sn, 90, 0,  3,  a_state);
Sleep(1000);
turn_arm(a_sn, -90, 0,  3,  a_state);
sleep(1000);
turn_arm(sm_sn, -45, 0,  4,  sm_state);
Sleep(1000);
turn_arm(sm_sn, 45, 0,  4,  sm_state);*/
ev3_uninit();
return ( 0 );
}