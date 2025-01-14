#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "mvt.h"

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
 
float compas(){
    uint8_t sn_compass;
    float value;
    if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_compass,0)){
      if ( !get_sensor_value0(sn_compass, &value )) {
        value = -1.0;
      }
      return(value);
    }
    return -1.0; //error
}

float sonar(){
    uint8_t sn_sonar;
    float value;
    float angl;
    if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0)){
      if ( !get_sensor_value0(sn_sonar, &value )) {
        value = -1.0;
      }
      if ( (value < 50.0) || (value > 2500.0) ){
        value = 0;
      }

      if (value < 100.0){
            mvt_forward(100, 35, -2, -2); //go backward
            angl = compas();
            if (angl > -20.0 && angl < 20.0){
                if (angl < 0){ //if go left
                    while (angl > -45.0){ 
                        mvt_forward(100, 0, 4, -4); //turn right
                        angl = compas();
                    }
                } else { // if go right
                    while (angl < 45.0){ 
                        mvt_forward(100, 0, -4, 4); //turn left
                        angl = compas();
                    }
                }
            } else {
                if (angl < 0){ // if go left
                    while (!(angl > 0 && angl < 5.0)) {
                        mvt_forward(100, 0, 4, -4); //turn right
                        angl = compas();
                    }
                } else { // if go right
                    while (!(angl < 0 && angl > -5.0)) {
                        mvt_forward(100, 0, -4, 4); //turn left
                        angl = compas();
                    }
                }
            }    
        }
        printf("bon");
        return value;
    }
    printf("bof");
    return -1.0; //error
}

int couleur(int stp){
    uint8_t sn_color;
    int val;
    if ( ev3_search_sensor( LEGO_EV3_COLOR, &sn_color, 0 )) {
        if ( !get_sensor_value( 0, sn_color, &val ) || ( val < 0 ) || ( val >= COLOR_COUNT )) {
            val = 0;
        }
        if (strcmp(color[val], "BLACK") == 0){
            return 2;
        }else if (strcmp(color[val], "GREEN") == 0 || strcmp(color[val], "YELLOW") == 0) {
            if (stp == 2 ) {
                return 3;
            }else {
                return 1;
            }
        }else if (strcmp(color[val], "WHITE") == 0) { // pour detecter le plot orange || strcmp(color[*val], "YELLOW") == 0
            return stp;
        }
      
    }
    return -1; //error
}

int touch(int time, int ramp, int l_vit, int r_vit){
    uint8_t sn_touch;
    float angl;

    if ( ev3_search_sensor( LEGO_EV3_TOUCH, &sn_touch, 0 )){
        if ( _check_pressed( sn_touch )){
            printf("j'ai touché un mur\n");
            Sleep( 100 );
            mvt_forward(time, ramp, l_vit, r_vit); //reculer
            angl = compas();
            if (angl > -20 && angl < 20) { // obstacle = plot
                if (angl < 0){
                    while (angl > -45.0){ 
                        mvt_forward(time, ramp, l_vit, -r_vit); //tourner a droite
                        angl = compas();
                        printf("Je suis coincé dans 5");
                    }
                } else { // si parti vers la droite
                    while (angl < 45.0){ 
                        mvt_forward(time, ramp, -l_vit, r_vit); //tourner a gauche
                        angl = compas();
                        printf("Je suis coincé dans 6");
                    }
                }
            } else {
                if (angl < 0){ // si partis vers la gauche
                    while (!(angl > 0 && angl < 5.0)) {
                        mvt_forward(time, ramp, l_vit, r_vit); //tourner a droite
                        angl = compas();
                        printf("Je suis coincé dans 7");
                    }
                } else { // si parti vers la droite
                    while (!(angl < 0 && angl > -5.0)) {
                        mvt_forward(time, ramp, l_vit, r_vit); //tourner a gauche
                        angl = compas();
                        printf("Je suis coincé dans 8");
                    }
                }
            }
        return 0;
        }
    }
    return -1; //error
}

void test_system(){
    int positif = 0;
    char fail[45] = ""; //"sonar compas color touch motor motor motor"
    int port;
    uint8_t sn_sonar;
    uint8_t sn_compass;
    uint8_t sn_color;
    uint8_t sn_touch;
    uint8_t sn;
    if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0)){
        positif += 1;
        const char *test1 = "sonar ";
        if (strlen(fail) + strlen(test1) < sizeof(fail)){
            strcat(fail, test1);
        }
    }
    if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_compass,0)){
        positif += 1;
        const char *test2 = "compas ";
        if (strlen(fail) + strlen(test2) < sizeof(fail)){
            strcat(fail, test2);
        }
    }
    if ( ev3_search_sensor( LEGO_EV3_COLOR, &sn_color, 0 )){
        positif += 1;
        const char *test3 = "color ";
        if (strlen(fail) + strlen(test3) < sizeof(fail)){
            strcat(fail, test3);
        }
    }
    if ( ev3_search_sensor( LEGO_EV3_TOUCH, &sn_touch, 0 )){
        positif += 1;
        const char *test4 = "touch";
        if (strlen(fail) + strlen(test4) < sizeof(fail)){
            strcat(fail, test4);
        }
    }
    for (port=65; port<69; port++){
        if ( ev3_search_tacho_plugged_in(port,0, &sn, 0 )) {
            if (port != M_SENSOR){
                positif +=1;
                const char *test5 = " motor";
                if (strlen(fail) + strlen(test5) < sizeof(fail)){
                    strcat(fail, test5);
                }
            }
        }
    }
    if (positif > 0) {
        printf("%d : %s\n", positif, fail);
    } else {
        printf("No sensor detected\n");
    }
    fflush( stdout );
}

void turn(int time, int ramp, int l_vit, int r_vit, int index, float degre) {
    float angl = compas();
    if (index == 0){ // si partis vers la gauche
                while (angl < degre){ 
                    mvt_forward(time, ramp, l_vit, -r_vit); //tourner a droite
                    angl = compas();
                }
            } else { // si parti vers la droite
                while (angl > -degre){ 
                    mvt_forward(time, ramp, -l_vit, r_vit); //tourner a gauche
                    angl = compas();
                }
            }
}
