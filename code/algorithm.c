initialize ev3, motors and sensors:

index = random draw between 0 and 1; // decide on which side the robot will go
flag = random draw between 0 and 1; // decide on which flag the robot will take

initialize sn and initial_angle; //variables
initialize orientation and angle_post; //lists

if flag = 0 { // flag on the right
    // go at full speed on the obstacle and go back and turn to the left of 90°
    go forward during 4s;
    go backward for 0.5s;
    turn to initial_angle -90;

    // move to reach the wall on the left and turn move to the right of 90°
    initialize touch sensor;
    while !touch {
        move forward during 0.1s;
    }
    turn to initial_angle;

    //open the arm and go forward until reach the wall
    open arm;
    while !touch {
        move forward during 0.1s;
    }

    //close the arm and turn of 180°
    close arm;
    initial_angle = initial_angle + 180;
    turn to initial_angle;

} else {
    // exit our area
    go forward for 1.2s;
    
    // turn to one side
    if index = 0 {
        turn to left;
    } else {
        turn to right;
    }
    while step != 3 { // loop to reach opponent area
        initialize sonar, gyroscope and color sensors;

        if step = 1 { // if before crossing the black line and if obstacle meet, robot go backward and turn a bit on one side
            if sonar < 10 {
                go back for 0.5s;
                if index = 0 {
                    turn a bit to the left; //check
                } else {
                    turn a bit to the right;
                }
            }

            if color = "black"{ // if black line detected turn on one side and go forward fo 0.7s
                step = 2;
                if index = 0 {
                    turn to initial_angle + 20;
                } else {
                    turn to initial_angle - 20;
                }
                go forward during 0.7s;
            }
        }

        if step = 2 { // if the black line has been crossed 
            if sonar < 10 { // if there is an obstacle go back turn of +/-90° go forward and turn to the inital position
                go back for 0.5s;
                if index = 0 {
                    turn to initial_angle + 90;
                    go forward during /*x*/ seconds;
                    turn to initial_angle
                } else {
                    turn to initial_angle - 90;
                    go forward during /*x*/ seconds;
                    turn to initial_angle
                }
            }

            if color = "green" or color = "yellow" { // if color detected is yellow or green the opponent area is reached 
                step = 3;
            }
        }
        go forward during 0.1s;
    }

    // go forward until you touch the wall, turn of 90 °, open the arm and go forward until you touch the wall
    while !touch {
        move forward during 0.1s;
    }

    turn to initial_angle + 90;
    open arm;

    while !touch {
        move forward during 0.1s;
    }

    // close the arm and turn of 180°
    close arm;
    go back for 1s;
    initial_angle = initial_angle + 180;
    turn to initial_angle;
}
if index = 0 { // depending of the index turn to initial_angle +/-20°
    turn  to initial_angle + 20;
} else {
    turn  to initial_angle - 20;
}

while step != 3 { // loop to reach our area
    initialize sonar, gyroscope and color sensors;

    if step = 1 { // if before crossing the black line and if obstacle meet, robot go backward and turn a bit on one side
        if sonar < 10 {
            go back for 0.5s;
            if index = 0 {
                turn a bit to the left;
            } else {
                turn a bit to the right;
            }
        }

        if color = "black"{ // if black line detected turn on one side and go forward fo 0.7s
            step = 2;
            go forward during 0.2s;
            if index = 0 {
                turn to initial_angle + 20;
            } else {
                turn to initial_angle - 20;
            }
        }
    }

    if step = 2 { // if the black line has been crossed 
        if sonar < 10 { // if there is an obstacle go back turn of +/-90° go forward and turn to the inital position
            go back for 0.5s;
            if index = 0 {
                turn to initial_angle + 90;
                go forward during /*x*/ seconds;
                turn to initial_angle
            } else {
                turn to initial_angle - 90;
                go forward during /*x*/ seconds;
                turn to initial_angle
            }
        }

        if color = "green" or color = "yellow" { // if color detected is yellow or green our area is reached 
            step = 3;
        }
    }
    go forward during 0.1s;
}

// go forward until you touch the wall, open the arm and go backward
while !touch { 
        move forward during 0.1s;
    }
open arm;
go back during 1s;