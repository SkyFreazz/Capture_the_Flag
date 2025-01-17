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

void main ()
{
  uint8_t sn_touch;
  uint8_t sn_color;
  uint8_t sn_compass;
  uint8_t sn_sonar;
  int val;
  float value;
  int i;
  char s[ 256 ];
  uint32_t n, ii;

  ev3_sensor_init();
  printf( "Found sensors:\n" );
  for ( i = 0; i < DESC_LIMIT; i++ ) {
    if ( ev3_sensor[ i ].type_inx != SENSOR_TYPE__NONE_ ) {
      printf( "  type = %s\n", ev3_sensor_type( ev3_sensor[ i ].type_inx ));
      printf( "  port = %s\n", ev3_sensor_port_name( i, s ));
      if ( get_sensor_mode( i, s, sizeof( s ))) {
        printf( "  mode = %s\n", s );
      }
      if ( get_sensor_num_values( i, &n )) {
        for ( ii = 0; ii < n; ii++ ) {
          if ( get_sensor_value( ii, i, &val )) {
            printf( "  value%d = %d\n", ii, val );
          }
        }
      }
    }
  }
  if ( ev3_search_sensor( LEGO_EV3_TOUCH, &sn_touch, 0 )) {
    printf( "TOUCH sensor is found, press BUTTON for EXIT...\n" );
  }
    for ( ; ; ){
    if ( ev3_search_sensor( LEGO_EV3_COLOR, &sn_color, 0 )) {
      printf( "COLOR sensor is found, reading COLOR...\n" );
      if ( !get_sensor_value( 0, sn_color, &val ) || ( val < 0 ) || ( val >= COLOR_COUNT )) {
        val = 0;
      }
      printf( "\r(%s) \n", color[ val ]);
      fflush( stdout );
    }
    if (ev3_search_sensor(LEGO_EV3_GYRO, &sn_compass,0)){
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