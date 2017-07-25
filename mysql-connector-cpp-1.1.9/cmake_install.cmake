# Install script for directory: /home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9

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
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE OPTIONAL FILES
    "/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/README"
    "/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/COPYING"
    "/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/Licenses_for_Third-Party_Components.txt"
    )
endif()

if(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/." TYPE FILE OPTIONAL FILES "/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/BUILDINFO")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/cppconn/cmake_install.cmake")
  include("/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/driver/cmake_install.cmake")
  include("/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/examples/cmake_install.cmake")
  include("/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/test/cmake_install.cmake")
  include("/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/test/framework/cmake_install.cmake")
  include("/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/test/CJUnitTestsPort/cmake_install.cmake")
  include("/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/test/unit/cmake_install.cmake")

endif()

if(CMAKE_INSTALL_COMPONENT)
  set(CMAKE_INSTALL_MANIFEST "install_manifest_${CMAKE_INSTALL_COMPONENT}.txt")
else()
  set(CMAKE_INSTALL_MANIFEST "install_manifest.txt")
endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
file(WRITE "/home/raymond/elec499/autopot-c++/mysql-connector-cpp-1.1.9/${CMAKE_INSTALL_MANIFEST}"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
