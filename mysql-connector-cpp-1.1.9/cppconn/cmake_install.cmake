# Install script for directory: /home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/cppconn

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/usr/local")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Install shared libraries without execute permission?
if(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  set(CMAKE_INSTALL_SO_NO_EXE "1")
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/cppconn" TYPE FILE FILES
    "/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/cppconn/build_config.h"
    "/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/cppconn/config.h"
    "/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/cppconn/connection.h"
    "/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/cppconn/datatype.h"
    "/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/cppconn/driver.h"
    "/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/cppconn/exception.h"
    "/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/cppconn/metadata.h"
    "/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/cppconn/parameter_metadata.h"
    "/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/cppconn/prepared_statement.h"
    "/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/cppconn/resultset.h"
    "/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/cppconn/resultset_metadata.h"
    "/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/cppconn/statement.h"
    "/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/cppconn/sqlstring.h"
    "/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/cppconn/warning.h"
    "/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/cppconn/version_info.h"
    "/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/cppconn/variant.h"
    )
endif()

