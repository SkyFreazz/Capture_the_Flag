#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "mvt.h"

// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif

static bool _check_pressed( uint8_t sn )
{
  int val;

  if ( sn == SENSOR__NONE_ ) {
    return ( ev3_read_keys(( uint8_t *) &val ) && ( val & EV3_KEY_UP ));
  }
  return ( get_sensor_value( 0, sn, &val ) && ( val != 0 ));
}

int catch_flag(int sn_sonar, int sn_touch, int sn_compass, uint8_t l_sn, uint8_t r_sn, uint8_t a_sn, FLAGS_T l_state, FLAGS_T r_state, FLAGS_T a_state, int initial_angle)
{
  int tol = 2; //set a tolerance for the angle
  int angle;
  int value;
  int last_angle;

  //Go back to initial angle (0°)
  while(ev3_search_sensor(HT_NXT_COMPASS, &sn_compass,0)){
    if ( !get_sensor_value0(sn_compass, &angle )) {
      angle = 0;
    }

    angle -= initial_angle;

    if (abs(angle % 360) <= tol){
      last_angle = angle;
      break;
    }

    angle = (angle % 360 + 360) % 360;

    if (angle >= 180){
      mvt_forward(l_sn, r_sn, 100, 0, -2, 2, l_state, r_state);
    } else {
      mvt_forward(l_sn, r_sn, 100, 0, 2, -2, l_state, r_state);
    }
  }

  //choose the flag to grab
  int flag = rand() % 2; //0: left, 1: right

  if (flag == 0){
  //Turn 90 left
    while(ev3_search_sensor(HT_NXT_COMPASS, &sn_compass,0)){
      if ( !get_sensor_value0(sn_compass, &angle )) {
        angle = 0;
      }

      angle -= initial_angle;
      angle += 90;

      if (abs(angle % 360) <= tol){
        last_angle = angle;
        break;
      }

      angle = (angle % 360 + 360) % 360;

      if (angle >= 180){
        mvt_forward(l_sn, r_sn, 100, 0, -2, 2, l_state, r_state);
      } else {
        mvt_forward(l_sn, r_sn, 100, 0, 2, -2, l_state, r_state);
      }
  }
  }

  //Move forward to the wall
  while (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0)){
      if (!get_sensor_value0(sn_sonar, &value )) {
        value = 0;
      }
      
      if (value <= 10 && _check_pressed( sn_touch )){
        break;
      }

      mvt_forward(l_sn, r_sn, 100, 0, -2, -2, l_state, r_state);
    }

  //Turn 90 right
  while(ev3_search_sensor(HT_NXT_COMPASS, &sn_compass,0)){
    if ( !get_sensor_value0(sn_compass, &angle )) {
      angle = 0;
    }

    angle -= initial_angle;
    angle -= 90;

    if (abs(angle % 360) <= tol){
      last_angle = angle;
      break;
    }

    angle = (angle % 360 + 360) % 360;

    if (angle >= 180){
      mvt_forward(l_sn, r_sn, 100, 0, -2, 2, l_state, r_state);
    } else {
      mvt_forward(l_sn, r_sn, 100, 0, 2, -2, l_state, r_state);
    }
  }

  //Open the clamp
  turn_angle(a_sn, 90, 0, 3, a_state, 67);

  //Move to the flag
  while (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0)){
      if (!get_sensor_value0(sn_sonar, &value )) {
        value = 0;
      }
      
      if (value <= 10 && _check_pressed( sn_touch )){
        break;
      }

      mvt_forward(l_sn, r_sn, 100, 0, -2, -2, l_state, r_state);
    }

  //Close the clamp
  turn_angle(a_sn, -90, 0, 3, a_state, 67);

  if (flag == 0){
    //Turn 180 right
    while(ev3_search_sensor(HT_NXT_COMPASS, &sn_compass,0)){
      if ( !get_sensor_value0(sn_compass, &angle )) {
        angle = 0;
      }

      angle -= initial_angle;
      angle -= 180;

      if (abs(angle % 360) <= tol){
        last_angle = angle;
        break;
      }

      angle -= initial_angle;
      angle = (angle % 360 + 360) % 360;

      if (angle >= 180){
        mvt_forward(l_sn, r_sn, 100, 0, -2, 2, l_state, r_state);
      } else {
        mvt_forward(l_sn, r_sn, 100, 0, 2, -2, l_state, r_state);
      }
    }
  } else{
    //Turn 90 right
    while(ev3_search_sensor(HT_NXT_COMPASS, &sn_compass,0)){
      if ( !get_sensor_value0(sn_compass, &angle )) {
        angle = 0;
      }

      angle -= initial_angle;
      angle -= 90;

      if (abs(angle % 360) <= 2){
        last_angle = angle;
        break;
      }

      angle = (angle % 360 + 360) % 360;

      if (angle >= 180){
        mvt_forward(l_sn, r_sn, 100, 0, -2, 2, l_state, r_state);
      } else {
        mvt_forward(l_sn, r_sn, 100, 0, 2, -2, l_state, r_state);
      }
    }
  }

  //Move forward to the wall
  while (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0)){
    if (!get_sensor_value0(sn_sonar, &value )) {
      value = 0;
    }
    
    if (value <= 10 && _check_pressed( sn_touch )){
      break;
    }

    mvt_forward(l_sn, r_sn, 100, 0, -2, -2, l_state, r_state);
  }
  
  //Open the clamp
  turn_angle(a_sn, 90, 0, 3, a_state, 67);

  //Move backward a bit
  mvt_forward(l_sn, r_sn, 500, 0, 2, 2, l_state, r_state);

return 0;
}