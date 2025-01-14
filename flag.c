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

int catch_flag(float initial_angle, int flag)
{
  float last_angle = initial_angle;
  float second_angle;

  printf("flag: %d\n", flag);

  if (flag == 0){
  //Turn 90 left
    turn_precise(last_angle - 90.0, 0);
    last_angle -= 90.0;
  }

  //Move forward to the wall
  forward_to_wall(150, last_angle, 0, 1);
  printf("hit first wall\n");
  mvt_forward(100, 0, -2, -2);

  //Turn 90 right
  turn_precise(last_angle + 90.0, 0);
  printf("turn done\n");
  last_angle += 90.0;

  //Open the clamp
  turn_arm(-3, 500);

  //Move to the flag
  forward_to_wall(150, last_angle, 0, 1);
  printf("touch the other wall");
  //mvt_forward(100, 0, -2, -2);


  //Close the clamp
  turn_arm(3, 500);

  mvt_forward(500, 100, -2, -2);

  //Turn to go back to area
  if (flag == 0){
    turn_precise(initial_angle - 183.0, 0);
    last_angle = initial_angle - 183.0;
    second_angle = initial_angle - 200.0;
  } else{
    turn_precise(initial_angle - 177.0, 0);
    last_angle = initial_angle - 177.0;
    second_angle = initial_angle - 150.0;
  }

  //Move forward to the wall of our base
  printf("let's go to base\n");
  forward_to_base(60.0, initial_angle - 180.0, last_angle, second_angle, 1, 2);
  forward_to_wall(150.0, initial_angle - 180.0, 1, 2);
  printf("touch wall\n");
  
  //Open the clamp
  turn_arm(-3, 500);

  //Move backward a bit
  mvt_forward(500, 0, -2, -2);

return 0;
}