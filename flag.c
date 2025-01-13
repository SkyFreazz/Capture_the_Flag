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

int catch_flag(float initial_angle)
{
  float last_angle = initial_angle;

  //Go back to initial angle (0°)
  turn_precise(initial_angle, 1);

  //choose the flag to grab
  int flag = 0; //rand() % 2; //0: left, 1: right

  printf("flag %i\n", flag);
  fflush(stdout);


  if (flag == 0){
  //Turn 90 left
    printf("go here\n");
    printf("angle to reach: %f", last_angle - 90.0);
    fflush(stdout);
    turn_precise(last_angle - 90.0, 0);
    last_angle -= 90.0;
  }

  printf("last_angle: %f", last_angle);
  fflush(stdout);

  //Move forward to the wall
  forward_to_wall(150, last_angle, 0, 1);
  mvt_forward(100, 0, -2, -2);

  //Turn 90 right
  printf("angle to reach: %f", last_angle + 90.0);
  fflush(stdout);
  turn_precise(last_angle + 90.0, 0);
  last_angle += 90.0;

  printf("last_angle: %f", last_angle);
  fflush(stdout);
  

  //Open the clamp
  turn_arm(-3, 1000);

  //Move to the flag
  forward_to_wall(150, last_angle, 0, 1);
  //mvt_forward(100, 0, -2, -2);


  //Close the clamp
  turn_arm(3, 800);


  //Turn to go back to area
  turn_precise(initial_angle - 183.0, 0);

  last_angle = initial_angle - 183.0;

  printf("last_angle: %f", last_angle);
  
  //Move forward to the wall
  forward_to_wall(150, last_angle, 0, 1);
  
  //Open the clamp
  turn_arm(-3, 1000);

  //Move backward a bit
  mvt_forward(500, 0, -2, -2);

return 0;
}