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
void * rightWheel(int milli){
  int port = 65;
  uint8_t sn;
  FLAGS_T state;
  if ( ev3_search_tacho_plugged_in(port,0, &sn, 0 )) {
    int max_speed;
    printf( "Right wheel running, run for %d milli...\n",milli);
    get_tacho_max_speed( sn, &max_speed );
    printf("  max speed = %d\n", max_speed );
    set_tacho_stop_action_inx( sn, TACHO_COAST );
    printf("Port number is  %d \n",port);
		if (milli<0){
			set_tacho_speed_sp( sn, -max_speed * 1 / 6 );
			milli = -milli;
		}
		else{
			set_tacho_speed_sp( sn, max_speed * 1 / 6 );
		}
    set_tacho_time_sp( sn, milli);
    set_tacho_command_inx( sn, TACHO_RUN_TIMED );
    /* Wait tacho stop */
    Sleep( milli );
    do {
      get_tacho_state_flags( sn, &state );
    } while ( state );
  } else {
    printf( "LEGO_EV3_M_MOTOR 1 is NOT found\n" );
  }
	printf("rightWheel Done !!!! \n");
	return;
}