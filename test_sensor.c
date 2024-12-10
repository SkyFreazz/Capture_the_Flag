#include <stdio.h>
#include <stdlib.h>
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

void main ()
{
    uint8_t sn_touch;
    uint8_t sn_color;
    uint8_t sn_compass;
    uint8_t sn_sonar;
    int val;
    float value;

    ev3_sensor_init();
    for ( ; ; ){
    if ( ev3_search_sensor( LEGO_EV3_COLOR, &sn_color, 0 )) {
      printf( "COLOR sensor is found, reading COLOR...\n" );
      get_sensor_value( 0, sn_color, &val );
      printf( "\r(%d$) \n", val);
      fflush( stdout );
    }
    if (ev3_search_sensor(HT_NXT_COMPASS, &sn_compass,0)){
      printf("COMPASS found, reading compass...\n");
      if ( !get_sensor_value0(sn_compass, &value )) {
        value = 0;
      }
      printf( "\r(%f) \n", value);
      fflush( stdout );
    }
    if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0)){
      printf("SONAR found, reading sonar...\n");
      if ( !get_sensor_value0(sn_sonar, &value )) {
        value = 0;
      }
      printf( "\r(%f) \n", value);
      fflush( stdout );
    }

    if ( _check_pressed( sn_touch )) break;
    Sleep( 200 );
    printf( "\r        " );
    fflush( stdout );
    if ( _check_pressed( sn_touch )) break;
    Sleep( 200 );
  }

  ev3_uninit();
  printf( "*** ( EV3 ) Bye! ***\n" );

}