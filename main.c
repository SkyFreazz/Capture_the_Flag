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
uint8_t sn_compass;
int e1 = 0;
float dist;
int stp;
int cross_2;
int orientation[2] = {-40.0, 40.0};
int tch;

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


ev3_sensor_init();

float initial_angle;

if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_compass,0)){
  if ( !get_sensor_value0(sn_compass, &initial_angle )) {
    initial_angle = 0;
  }
  printf("initial_angle: %f", initial_angle);
}

test_system();

srand(time(NULL)); //demarage tirage alea
int index = rand() % 2 ;

mvt_motor(1500, 200, 2, 2); // sortir du carré

if (index == 0) {
    while (compas() > orientation [index] ){
            mvt_motor(100, 35, -4, 4); // tourner vers la gauche
    }
} else{
    while (compas() < orientation [index] ){
            mvt_motor(100, 35, 4, -4); // tourner vers la droite
    }
}
fflush( stdout );
stp = 1;
cross_2 = 0;
while (!e1){
    if (stp == 3){
        e1 = 1;
    } 
    //utilise tous les capteurs
    dist = sonar();
    stp = couleur(stp);
    tch = touch(100, 35, 4, 4);
    if (dist == -1.0 || tch == -1){ //si capteur non detecté
        e1 = 1;
    }
    if ((stp == 2) && (cross_2 ==0)){ //quand on arrive a la ligne du centre
        turn(100, 35, 4, 4, index, 20.0);
        printf("j'ai atteint le centre\n");
        cross_2 +=1;
    }
    mvt_motor(100, 0, 3, 3); //avance 
    fflush( stdout );
}
fflush( stdout );


catch_flag(initial_angle);


ev3_uninit();
return ( 0 );
}