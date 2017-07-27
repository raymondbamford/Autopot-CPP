#!/bin/bash

# echo "Hello, $1" 

#var1=$(setserial -g /dev/ttyACM[0123456789] | cut -c 1-12)
var1=$(setserial -g /dev/ttyUSB[0123456789] | cut -c 1-12)

#echo "test"
#echo "$var1"

./connector $var1
