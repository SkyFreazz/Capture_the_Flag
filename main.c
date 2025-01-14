#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "mvt.h"
#include "flag.h"
#include "step1.h"


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
uint8_t sn_compass;
int orientation[2] = {-36.0, 36.0};
int angle_postb[2] = {20.0, -20.0};
float initial_angle;

#ifndef __ARM_ARCH_4T__
  /* Disable auto-detection of the brick (you have to set the correct address below) */
  ev3_brick_addr = "192.168.79.191";

#endif
  if ( ev3_init() == -1 ) return ( 1 );

#ifndef __ARM_ARCH_4T__
  printf( "The EV3 brick auto-detection is DISABLED,\nwaiting %s online with plugged tacho...\n", ev3_brick_addr );

#else
  printf( "Waiting tachos is plugged...\n" );

#endif
  while ( ev3_tacho_init() < 3) Sleep( 1000 );


ev3_sensor_init();

if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_compass,0)){
  if ( !get_sensor_value0(sn_compass, &initial_angle )) {
    initial_angle = 0;
  }
}

srand(time(NULL)); //random initialization
int index = rand() % 2 ;
int flag = rand() %2;

if (flag == 0){
  mvt_forward(4000, 200, 1, 1); // get out of our area
  mvt_forward(2000, 200, -1,-1);
} else {
  mvt_forward(1200, 250, 1, 1);
}

float start_angle = initial_angle + orientation[index];
float second_angle = initial_angle + angle_postb[index];

turn_precise(start_angle, 1);

forward_to_base(60.0, initial_angle, start_angle, second_angle, 1, 2);


printf("Arrived in the other base\n");
//We arrive in the opponent's area, let's catch a flag

catch_flag(initial_angle, flag);


ev3_uninit();
return ( 0 );
}