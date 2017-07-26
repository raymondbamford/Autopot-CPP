#!/bin/bash

# echo "Hello, $1" 

unparsedString=$(setserial -g /dev/ttyACM[0123456789])

for i in {0..11}
do
  comports[i]=$unparsedString[i]
done

echo "test"
echo $comports
