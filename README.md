# <span style="color:red"> <center> Capture and defend the flags game, Team 2 : ***CRASH!*** </center> </span>


## 1. Introduction

Welcome to our project named ***CATCH!***. This report is devided in 4 parts :  

- [Presentation our our robot](#2-presentation-of-the-robot)

- [Presentation of our git](#3-presentation-of-our-git)

- [How to launch the code](#4-how-to-use-the-code)

- [Contribution to the project](#5-contribution)

## 2. Presentation of the robot

![CRASH!](https://github.com/SkyFreazz/Capture_the_Flag/tree/main/pictures/crash.jpg?ref_type=heads)

***CRASH!*** is a robot capable of picking up a flag located in each corner of the opponent's base and bringing it back to its base. Thanks to its 3 motors, it can turn the two wheels independently of each other and activate its arm to grab the flag. He's also equipped with 4 sensors: a sonar, a gyroscope, a color sensor and a touch sensor, enabling him to find his way around the play area and dodge any objects that get in his way. And if these objects give him no other option, thanks to his armored tank reinforcement and daredevil mentality, he can charge right at them and smash his way through. 

![image of the robot](https://github.com/SkyFreazz/Capture_the_Flag/tree/main/pictures/lest_arm.jpg?ref_type=heads)

In short, ***CRASH!*** is **THE** solution for getting to your opponent's flag the most efficient way without fear of being blown to smithereens.

## 3. Presentation of our git

The git is divided in 3 directories:

- [code](https://github.com/SkyFreazz/Capture_the_Flag/tree/main/code?ref_type=heads): this is where all the code of the project is. You can find the Makefile to be able to launch the code ([click to see how it works](#how-to-use-the-code)), the main code with all the related functions, and a code to test the sensors.

- [pictures](https://github.com/SkyFreazz/Capture_the_Flag/tree/main/pictures?ref_type=heads): you can find some pictures of our robots from different side and also the pictures of the different sensors we are using in our robot.

- [videos](https://github.com/SkyFreazz/Capture_the_Flag/tree/main/videos?ref_type=heads): you can find some videos of how **CRASH!** works on the field.

## 4. How to use the code

>  <span style="color:GREEN"> **[NOTE]** </span>  
> All the code is in the code directory. You can download it and use the directory as source directory.

1. Intall the ev3 docker image
```
$ docker pull ev3dev/debian-stretch-cross
```
2. Tag the docker image
```
$ docker tag ev3dev/debian-stretch-cross ev3cc
```
3. Install ev3 lib, put them in the project directory, and start the docker image
```
$ git clone https://github.com/in4lio/ev3dev-c
$ docker run --rm -it -h ev3 -v PATH/TO/PROJECT/:/src -w /src ev3cc /bin/bash
```
4. In the container, run the following commands
```
[Shell in container]$ cd ev3dev-c/source/ev3/&&make&&sudo make install&&make shared&&sudo make shared-install
```
5. Run the Makefile
```
[Shell in container]$ cd /src/code
[Shell in container]$ make all
```
> <span style="color:GREEN"> **[NOTE]** </span>  
> 'make all' compile both 'main.c' and 'test_sensor.c'. You can also run 'make main' or 'make test' to compile only one. You can also run 'make clean' to remove all .o files and 'main' and 'test_sensor.

6. Leave the docker and copy the code on the robot
```
[Shell in container]$ exit
$scp $(pwd)/code/main robot@IP_addr_of_your_robot:/home/robot
```

> <span style="color:red"> **[WARNING]** </span>  
> Your computer and you robot needs to be connected on the same connection!

7. Connect to the robot and launch the code
```
$ssh robot@IP_of_your_robot
[Shell of your robot]$ ./main
```

## 5. Contribution

- MOSQUEDA Leonardo : was involved in the design of the entire robot and in the design of the code needed to leave the base and join the opposing base. He also tidied up the git, took photos and videos of the robot and wrote the algorithm in pseudo code.

- VARINOT Lilian : has designed the code to catch one of the two flags when you arrive at the opposing base and wrote this report. He also put the two codes together, made the code more readable and optimised it with Leonardo's help.
