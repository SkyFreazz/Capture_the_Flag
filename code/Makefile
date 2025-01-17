
export LD_LIBRARY_PATH=~/ev3dev-c/lib

all:lib/flag.h lib/mvt.h
	arm-linux-gnueabi-gcc -I/src/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c functions/flag.c -o functions/flag.o
	arm-linux-gnueabi-gcc -I/src/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c functions/mvt.c -o functions/mvt.o
	arm-linux-gnueabi-gcc -I/src/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c main.c -o main.o
	arm-linux-gnueabi-gcc main.o functions/flag.o functions/mvt.o -Wall -lm -lev3dev-c -o main

	arm-linux-gnueabi-gcc -I/src/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_sensor.c -o test_sensor.o
	arm-linux-gnueabi-gcc test_sensor.o -Wall -lm -lev3dev-c -o test_sensor

main: lib/flag.h lib/mvt.h
	arm-linux-gnueabi-gcc -I/src/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c functions/flag.c -o functions/flag.o
	arm-linux-gnueabi-gcc -I/src/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c functions/mvt.c -o functions/mvt.o
	arm-linux-gnueabi-gcc -I/src/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c main.c -o main.o
	arm-linux-gnueabi-gcc main.o functions/flag.o functions/mvt.o -Wall -lm -lev3dev-c -o main

test_sensor:
	arm-linux-gnueabi-gcc -I/src/ev3dev-c/source/ev3 -O2 -std=gnu99 -W -Wall -Wno-comment -c test_sensor.c -o test_sensor.o
	arm-linux-gnueabi-gcc test_sensor.o -Wall -lm -lev3dev-c -o test_sensor

clean:
	rm -f functions/*.o lib/*.o main.o test_sensor.o test_sensor main
