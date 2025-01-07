#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> //cherche random
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "step1s.h"

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

    int e1 = 0;
    float dist;
    float angl;
    int stp;
    float start_angl;
    int cross_2;
    float orientation[2] = {-30.0, 30.0};
    float direction;


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
    test_system();
    printf( "Found sensors:\n" );

    srand(time(NULL)); //demarage tirage alea
    int index = rand() % 2 ;
    mvt_motor(1500, 200, 2, 2); // sortir du carré
    printf("je sors du carré");
    start_angl = compas();
    if (index == 0) {
        while (compas() > orientation [index] ){
                mvt_motor(100, 35, -4, 4); // tourner vers la gauche
                printf("je suis a gauche");
        }
    } else{
        while (compas() < orientation [index] ){
                mvt_motor(100, 35, 4, -4); // tourner vers la droite
                printf("je suis a droite");
        }
    }
    direction = orientation [index];
    stp = 1;
    cross_2 = 0;
    while (!e1){
        if (stp == 3){
            e1 = 1;
        } 
        //utilise tous les capteurs
        dist = sonar();
        stp = couleur(stp);
        touch(100, 35, 4, 4);
        if ((stp == 2) && (cross_2 ==0)){ //quand on arrive a la ligne du centre
            printf("j'ai atteint le centre'");
            if (direction < 0) {
                direction = 20;
                turn(100, 35, 4, 4, index, direction);
            } else {
                direction = -20;
                turn(100, 35, 4, 4, index, direction);
            }
            cross_2 +=1;
        }
        if (dist < 100.0){
            mvt_motor( 100, 35, -2, -2); //reculer
            angl = compas();
            turn(100, 35, 4, 4, index, 10);
            printf("le mur est trop proche");
        }
        stay(100, 0, 2, 3, direction, 3.0);
        //mvt_motor(100, 0, 2, 2);

    } 
    ev3_uninit();
}