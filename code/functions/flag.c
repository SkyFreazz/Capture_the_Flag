#include <stdio.h>
#include <stdlib.h>
#include "ev3.h"
#include "ev3_port.h"
#include "ev3_tacho.h"
#include "ev3_sensor.h"
#include "../lib/mvt.h"

// WIN32 /////////////////////////////////////////
#ifdef __WIN32__

#include <windows.h>

// UNIX //////////////////////////////////////////
#else

#include <unistd.h>
#define Sleep( msec ) usleep(( msec ) * 1000 )

//////////////////////////////////////////////////
#endif

int catch_flag(float initial_angle, int flag)
{
  float last_angle = initial_angle;
  float second_angle;
  uint8_t sn_sonar;
  float value;

  printf("flag: %d\n", flag);

  if (flag == 0){
  //Turn 90 left
    turn_precise(last_angle - 90.0, 0);
    last_angle -= 90.0;
  }

  //Move forward to the wall
  forward_to_wall(150, last_angle, 0, 1);
  printf("hit first wall\n");
  while(true){
    if (ev3_search_sensor(LEGO_EV3_US, &sn_sonar,0)){
      if (!get_sensor_value0(sn_sonar, &value )) {
          value = 0;
        }
    }
    
    if (value < 0.0){
      mvt_forward(100, 0, -4, -4);
    } else if (value > 55.0){
      mvt_forward(100, 0, 4, 4);      
    } else {
      break;
    }
  }
  

  //Turn 90 right
  turn_precise(last_angle + 89.0, 0);
  printf("turn done\n");
  last_angle += 90.0;

  //Open the clamp
  turn_arm(-3, 500);

  //Move to the flag
  forward_to_wall(150, last_angle, 0, 1);
  printf("touch the other wall");


  //Close the clamp
  turn_arm(3, 500);
  mvt_forward(100, 0, -2, -2);

  //Turn to go back to area
  if (flag == 0){
    mvt_forward(1000, 300, -2, -5);
    turn_precise(initial_angle - 182.0, 0);
    last_angle = initial_angle - 182.0;
    second_angle = initial_angle - 200.0;
  } else{
    mvt_forward(1000, 300, -5, -2);
    turn_precise(initial_angle - 180.0, 0);
    last_angle = initial_angle - 180.0;
    second_angle = initial_angle - 150.0;
  }

  //Move forward to the wall of our base (first to the base and the to the wall)
  printf("let's go to base\n");
  mvt_forward(1000, 200, 1, 1);
  forward_to_base(40.0, initial_angle - 180.0, last_angle, second_angle, 1, 2);
  forward_to_wall(150.0, initial_angle - 180.0, 1, 2);
  printf("touch wall\n");
  
  //Open the clamp
  turn_arm(-3, 500);

  //Move backward a bit
  mvt_forward(1000, 0, -2, -2);

return 0;
}