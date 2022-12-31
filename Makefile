#*************************************************************
# Class:  CSC-615-01 Fall 2022
# Names: Faiyaz Chaudhury, Hector Magallanes,
#        Briget Soriano, Emanuel Francis
# Student IDs: 920273520, 921660295, 921163422, 917491268 
# GitHub Name: faiyazc
# Group Name: 0xFF Pi
# Project: Self Driving Car
#
# File: Makefile
#*************************************************************/
main : bin/DEV_Config.o bin/dev_hardware_i2c.o bin/main.o bin/PCA9685.o bin/sysfs_gpio.o bin/EngineDriver.o
	gcc -Wall -pthread bin/DEV_Config.o bin/dev_hardware_i2c.o bin/main.o bin/PCA9685.o bin/sysfs_gpio.o bin/EngineDriver.o -o $@ -lm -lpigpio -lrt

bin/main.o : main.c
	gcc -Wall -pthread -c main.c -o $@ -I ./engine_drivers

bin/EngineDriver.o : ./engine_drivers/EngineDriver.c
	gcc -c ./engine_drivers/EngineDriver.c -o $@

bin/DEV_Config.o : ./engine_drivers/DEV_Config.c
	gcc -c ./engine_drivers/DEV_Config.c -o $@

bin/dev_hardware_i2c.o : ./engine_drivers/dev_hardware_i2c.c
	gcc -c ./engine_drivers/dev_hardware_i2c.c -o $@

bin/sysfs_gpio.o : ./engine_drivers/sysfs_gpio.c
	gcc -c ./engine_drivers/sysfs_gpio.c -o $@

bin/PCA9685.o : ./engine_drivers/PCA9685.c
	gcc -c ./engine_drivers/PCA9685.c -o $@

run:
	sudo ./main

clean:
	rm ./bin/*.*
	rm main
