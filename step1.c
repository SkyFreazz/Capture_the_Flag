#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

void mvt_motor(uint8_t l_sn, uint8_t r_sn, int time, int ramp, int l_vit, int r_vit, FLAGS_T l_state, FLAGS_T r_state)
{
    if ( ev3_search_tacho_plugged_in(L_WHEEL,0, &l_sn, 0 ) && ev3_search_tacho_plugged_in(R_WHEEL,0, &r_sn, 0 ) ){
      int l_max_speed;
      int r_max_speed;
      get_tacho_max_speed( l_sn, &l_max_speed );
      get_tacho_max_speed( r_sn, &r_max_speed );
      set_tacho_stop_action_inx( l_sn, TACHO_COAST );
      set_tacho_stop_action_inx( r_sn, TACHO_COAST );
      l_max_speed = -l_max_speed;
      r_max_speed = -r_max_speed;
      set_tacho_speed_sp( l_sn, l_max_speed / l_vit );
      set_tacho_speed_sp( r_sn, r_max_speed / r_vit );
      set_tacho_time_sp( l_sn, time );
      set_tacho_time_sp( r_sn, time );
      set_tacho_ramp_up_sp( l_sn, ramp );
      set_tacho_ramp_up_sp( r_sn, ramp );
      set_tacho_ramp_down_sp( l_sn, ramp );
      set_tacho_ramp_down_sp( r_sn, ramp );
      set_tacho_command_inx( l_sn, TACHO_RUN_TIMED );
      set_tacho_command_inx( r_sn, TACHO_RUN_TIMED );
      do {
        get_tacho_state_flags( l_sn, &l_state );
        get_tacho_state_flags( r_sn, &r_state );
      } while ( l_state && r_state ); // d'apres gpt (l_state || r_state)
    } else {
      printf( "LEGO_EV3_M_MOTOR 1 is NOT found\n" );
    }
    return;
}

float sonar(uint8_t sn_sonar, float value){
    if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0)){
      if ( !get_sensor_value0(sn_sonar, &value )) {
        value = -1.0;
      }
      if ((value <250.0) || (value > 5.0)){
        //fflush( stdout ); //seulement utile pour les print 
        return(value);
      }else {
        //fflush( stdout ); //seulement utile pour les print 
        return 0 ;
      }
      
    }
    return -1.0; //error
}

float compas(uint8_t sn_compass, float value){
    if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_compass,0)){
      if ( !get_sensor_value0(sn_compass, &value )) {
        value = -1.0;
      }
      //fflush( stdout ); //seulement utile pour les print 
      return(value);
    }
    return -1.0; //error
}

int couleur(uint8_t sn_color, int val, int stp){
    if ( ev3_search_sensor( LEGO_EV3_COLOR, &sn_color, 0 )) {
        if ( !get_sensor_value( 0, sn_color, &val ) || ( val < 0 ) || ( val >= COLOR_COUNT )) {
            val = 0;
        }
        //fflush( stdout ); //seulement utile pour les print 
        if (strcmp(color[val], "BLACK") == 0){
            return 2;
        }else if (strcmp(color[val], "GREEN") == 0 || strcmp(color[val], "YELLOW") == 0) {
            if (stp == 2 ) {
                return 3; // return (stp == 2) ? 3 : 1; //d'apres gpt
            }else {
                return 1;
            }
        }else if (strcmp(color[val], "WHITE") == 0) { // pour detecter le plot orange || strcmp(color[*val], "YELLOW") == 0
            return stp;
        }
      
    }
    return -1; //error
}

int touch(uint8_t sn_compass, uint8_t sn_touch, uint8_t l_sn, uint8_t r_sn, int time, int ramp, int l_vit, int r_vit, FLAGS_T l_state, FLAGS_T r_state){
    float angl;
    float value;
    printf("j'ai touché un mur'");
    if ( ev3_search_sensor( LEGO_EV3_TOUCH, &sn_touch, 0 )){
        if ( _check_pressed( sn_touch )){
            Sleep( 100 );
            mvt_motor(l_sn, r_sn, time, ramp, l_vit, r_vit, l_state, r_state); //reculer
            angl = compas(sn_compass, value);
            if (angl < 0){
                while (!(angl > 0 && angl < 5.0)) {
                    mvt_motor(l_sn, r_sn, time, ramp, l_vit, r_vit, l_state, r_state); //tourner a droite
                    angl = compas(sn_compass, value);
                }
            } else {
                while (!(angl < 0 && angl > -5.0)) {
                    mvt_motor(l_sn, r_sn, time, ramp, l_vit, r_vit, l_state, r_state); //tourner a gauche
                    angl = compas(sn_compass, value);
                }
            }
        return 0;
        }
    return -1; //error
    }
}

void test_system(uint8_t sn_sonar, uint8_t sn_compass, uint8_t sn_color, uint8_t sn_touch, uint8_t sn){
    int positif = 0;
    char fail[50] = ""; //"sonar compas color touch motor motor motor motor"
    int port;
    
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
            positif +=1;
            const char *test5 = " motor";
            if (strlen(fail) + strlen(test5) < sizeof(fail)){
                strcat(fail, test5);
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

void turn(uint8_t sn_compass, uint8_t l_sn, uint8_t r_sn, int time, int ramp, int l_vit, int r_vit, FLAGS_T l_state, FLAGS_T r_state, int index, float degre) {
    float value;
    float angl = compas(sn_compass, value);
    if (index == 0){ // si partis vers la gauche
                while (angl < degre){ 
                    mvt_motor(l_sn, r_sn, time, ramp, l_vit, -r_vit, l_state, r_state); //tourner a droite
                    angl = compas(sn_compass, value);
                }
            } else { // si parti vers la droite
                while (angl > -degre){ 
                    mvt_motor(l_sn, r_sn, time, ramp, -l_vit, r_vit, l_state, r_state); //tourner a gauche
                    angl = compas(sn_compass, value);
                }
            }
}

void stay(uint8_t sn_compass, uint8_t l_sn, uint8_t r_sn, int time, int ramp, int max_vit, int min_vit, FLAGS_T l_state, FLAGS_T r_state, int index, float degre, float ecart) {
    float value;
    float angl = compas(sn_compass, value);
    if (angl + ecart > degre){ // si partis vers la gauche
        mvt_motor(l_sn, r_sn, time, ramp, max_vit, min_vit, l_state, r_state); //tourner a droite
    } else if (angl + ecart < degre) { // si parti vers la droite
        mvt_motor(l_sn, r_sn, time, ramp, min_vit, max_vit, l_state, r_state); //tourner a gauche
    }
}