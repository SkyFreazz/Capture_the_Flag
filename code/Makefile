
export LD_LIBRARY_PATH=~/ev3dev-c/lib

all:
	arm-linux-gnueabi-gcc -I/src/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c flag.c -o flag.o
	arm-linux-gnueabi-gcc -I/src/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c mvt.c -o mvt.o
	arm-linux-gnueabi-gcc -I/src/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c main.c -o main.o
	arm-linux-gnueabi-gcc main.o flag.o mvt.o -Wall -lm -lev3dev-c -o main

main:
	arm-linux-gnueabi-gcc -I/src/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c flag.c -o flag.o
	arm-linux-gnueabi-gcc -I/src/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c mvt.c -o mvt.o
	arm-linux-gnueabi-gcc -I/src/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c main.c -o main.o
	arm-linux-gnueabi-gcc main.o flag.o mvt.o -Wall -lm -lev3dev-c -o main

test:
	arm-linux-gnueabi-gcc -I/src/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c step1.c -o step1.o
	arm-linux-gnueabi-gcc -I/src/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test.c -o test.o
	arm-linux-gnueabi-gcc step1.o test.o -Wall -lm -lev3dev-c -o test

test2:
	arm-linux-gnueabi-gcc -I/src/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c step1s.c -o step1s.o
	arm-linux-gnueabi-gcc -I/src/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test2.c -o test2.o
	arm-linux-gnueabi-gcc step1s.o test2.o -Wall -lm -lev3dev-c -o test2

test_sensor:
	arm-linux-gnueabi-gcc -I/src/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_sensor.c -o test_sensor.o
	arm-linux-gnueabi-gcc test_sensor.o -Wall -lm -lev3dev-c -o test_sensor


