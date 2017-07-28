#!/bin/bash

g++ -o connector connector.cpp rs232.o -I/usr/local/include -I/usr/local/include/cppconn -lmysqlcppconn -Wall -O2 -Wextra -std=c++11
