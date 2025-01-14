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
    if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0)){
      if ( !get_sensor_value0(sn_sonar, &value )) {
        value = -1.0;
      }
      if ( (value < 40.0) || (value > 2500.0) ){
        value = 0;
      }
    
    return value;
    }
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
        }else { // pour detecter le plot orange || strcmp(color[*val], "YELLOW") == 0
            return stp;
        }
      
    }
    return -1; //error
}

void test_system(){
    int positif = 0;
    char fail[45] = ""; //"sonar compas color touch motor motor motor"
    uint8_t sn_sonar;
    uint8_t sn_compass;
    uint8_t sn_color;
    uint8_t sn_touch;
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
    if (positif > 0) {
        printf("%d : %s\n", positif, fail);
    } else {
        printf("No sensor detected\n");
    }
    fflush( stdout );
}
