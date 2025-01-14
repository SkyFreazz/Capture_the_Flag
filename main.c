#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
int orientation[2] = {-40.0, 40.0};
int tch;
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

test_system();

srand(time(NULL)); //random initialization
int index = rand() % 2 ;

mvt_forward(1500, 200, 2, 2); // get out of our area

if (index == 0) {
    while (compas() > orientation [index] ){
            mvt_forward(100, 35, -4, 4); // turn left
    }
} else{
    while (compas() < orientation [index] ){
            mvt_forward(100, 35, 4, -4); // turn right
    }
}

while (stp != 3){

    printf("stp: %d", stp);
    //get all the value of the sensor
    dist = sonar();
    stp = couleur(stp);
    tch = touch(100, 35, 4, 4);

    printf("dist: %f\ntch: %d", dist, tch);

    if (dist == -1.0 || tch == -1){ //no sensor
        printf("here");
        break;
    }
    if ((stp == 2) && (cross_2 ==0)){ //we are in the middle
        turn(100, 35, 4, 4, index, 20.0);
        cross_2 +=1;
    }
    mvt_forward(100, 0, 3, 3);
    printf("stp end: %d", stp);
}

//We arrive in the opponent's area, let's catch a flag

catch_flag(initial_angle);


ev3_uninit();
return ( 0 );
}