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
  int tol = 1; //set a tolerance for the angle
  float last_angle;

  //Go back to initial angle (0°)
  last_angle = turn_precise(initial_angle, tol);

  //choose the flag to grab
  int flag = 0; //rand() % 2; //0: left, 1: right

  printf("flag %i\n", flag);
  fflush(stdout);


  if (flag == 0){
  //Turn 90 left
    printf("go here\n");
    printf("angle to reach: %f", initial_angle - 90.0);
    fflush(stdout);
    last_angle = turn_precise(initial_angle - 90.0, 2);
  }

  printf("last_angle: %f", last_angle);
  fflush(stdout);

  //Move forward to the wall
  forward_to_wall(150, last_angle, 1, 2);

  //Turn 90 right
  printf("angle to reach: %f", last_angle + 90.0);
  fflush(stdout);
  last_angle = turn_precise(last_angle + 90.0, tol);

  printf("last_angle: %f", last_angle);
  fflush(stdout);
  

  //Open the clamp
  turn_angle(75, 0, 3, 67);

  //Move to the flag
  forward_to_wall(150, last_angle, tol, tol);

  //Close the clamp
  turn_angle(-75, 0, 3, 67);


  //Turn to go back to area
  last_angle = turn_precise(initial_angle - 180.0, tol);

  printf("last_angle: %f", last_angle);
  
  //Move forward to the wall
  forward_to_wall(150, last_angle, 1, 1);
  
  //Open the clamp
  turn_angle(75, 0, 3, 67);

  //Move backward a bit
  mvt_forward(500, 0, -2, -2);

return 0;
}