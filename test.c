#include <stdio.h>
#include <stdlib.h>
#include <time.h> //cherche random
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
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

int main( void )
{

    int i;
    FLAGS_T l_state;
    FLAGS_T r_state;
    FLAGS_T a_state;
    FLAGS_T sm_state;
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
    int e1 = 0;
    float dist;
    float angl;
    int stp;
    float start_angl:
    int cross_2;
    int orientation[2] = {-30, 30};

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

    printf( "Found tacho motors:\n" );

    ev3_sensor_init();
    test_system( sn_sonar,  sn_compass,  sn_color,  sn_touch,  l_sn);
    printf( "Found sensors:\n" );

    srand(time(NULL)); //demarage tirage alea
    int index = rand() % 2 ;
    mvt_motor(l_sn, r_sn, 1500, 2000, 1000, 2, l_state, r_state); // sortir du carré
    start_angl = compas( sn_compass, value);
    if (index == 0) {
        while (compas( sn_compass, value) > orientation [index] ){
                mvt_motor(l_sn, r_sn, 500, 400, -4, 4, l_state, r_state); // tourner vers la gauche
        }
    } else{
        while (compas( sn_compass, value) < orientation [index] ){
                mvt_motor(l_sn, r_sn, 500, 400, 4, -4, l_state, r_state); // tourner vers la droite
        }
    }
/*
    stp = 1;
    cross_2 = 0;
    while (!e1){
        if (stp == 3){
            e1 = 1;
        } 
        //utilise tous les capteurs
        dist = sonar( sn_sonar, value);
        stp = color( sn_color, val, stp);
        touch( sn_touch, l_sn, r_sn, 500, 400, 4, 4, l_state, r_state);
        if ((stp == 2) && (cross_2 ==0)){ //quand on arrive a la ligne du centre
            /*if (index == 0){ // si parti vers la gauche
                while (angl < 0){ 
                    mvt_motor(l_sn, r_sn, time, ramp, vit, l_state, r_state); //tourner a droite
                    angl = compas( sn_compass, value);
                }
            } else { // si parti vers la droite
                while (angl > 0){ 
                    mvt_motor(l_sn, r_sn, time, ramp, vit, l_state, r_state); //tourner a gauche
                    angl = compas (sn_compass,  value);
                }
            }
            turn(sn_compass, l_sn, r_sn, 500, 400, 4, 4, l_state, r_state, index, 20.0);
            cross_2 +=1;
        }
        if (dist < 10.0){
            mvt_motor(l_sn, r_sn, 500, 400, 4, 4, l_state, r_state); //reculer
            angl = compas( sn_compass,  value);
            turn(sn_compass, l_sn, r_sn, 500, 400, 4, 4, l_state, r_state, index, 10);
            /*
            if (index == 0){ // si partis vers la gauche
                while (angl < 0){ 
                    mvt_motor(l_sn, r_sn, time, ramp, vit, l_state, r_state); //tourner a droite
                    angl = compas( sn_compass, value);
                }
            } else { // si parti vers la droite
                while (angl > 0){ 
                    mvt_motor(l_sn, r_sn, time, ramp, vit, l_state, r_state); //tourner a gauche
                    angl = compas( sn_compass, value);
                }
            }
        }
        mvt_motor(l_sn, r_sn, 500, 200, 2, 2, l_state, r_state); //avance pendant 0.5s ?

    }*/
    ev3_uninit();
}