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
float dist;
int stp = 1;
int cross_2 = 0;
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

/*if (flag == 0){
  mvt_forward(4000, 200, 1, 1); // get out of our area
  mvt_forward(2000, 200, -1,-1);
} else {*/
  mvt_forward(1200, 250, 1, 1);
//}

turn_precise(initial_angle + orientation[index], 1);

while (stp != 3){

    //get all the value of the sensor
    dist = sonar();
    stp = couleur(stp);

    if (dist == -1.0){ //no sensor
        break;
    }

    if (dist <= 60.0){ //there's an object
      if (stp == 1){
        mvt_forward(500, 100, -2, -2);
        turn_precise(initial_angle + orientation[(index+1) % 2], 1);
      }

      if(stp == 2){
        mvt_forward(200, 50, -2, -2);
        if(index){
          turn_precise(initial_angle - 90.0, 1);
        } else {
          turn_precise(initial_angle + 90.0, 1);
        }
        mvt_forward(1000, 250, 2, 2);
        turn_precise(initial_angle, 1);
      }
    }
    if ((stp == 2) && (cross_2 <= 1)){ //we are in the middle
      if(fabs(initial_angle - compas()) >= 5){
        turn_precise(initial_angle + angle_postb[index], 1);
        cross_2 +=1;
      }      
      if (cross_2 == 1){
        mvt_forward(500, 50, 1, 1);
        cross_2 +=1;
      }
    }
    mvt_forward(100, 0, 4, 4);
}

//We arrive in the opponent's area, let's catch a flag

catch_flag(initial_angle, flag);


ev3_uninit();
return ( 0 );
}