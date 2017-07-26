#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unistd.h>
#include <string>
/* uncomment for applications that use vectors */
/*#include <vector>*/

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "rs232.h"

#define SQL_HOST "rbamford0.ddns.net"
#define SQL_USER "autopot"
#define SQL_PASS "12345"
#define SQL_DB "autopot"

using namespace std;


string check_Comport(int cport_nr){
  int n;
  unsigned char buf[4096];

  string input="";
  n = RS232_PollComport(cport_nr, buf, 4095);

  if(n > 0){
    buf[n] = 0;   /* always put a "null" at the end of a string! */

    for(int i=0; i < n; i++){
      if(buf[i] < 32){  /* replace unreadable control-codes by dots */
        buf[i] = '.';
      }
    }
    input = (char *)buf;
    cout << "received "<< n <<" bytes: " << input << endl;
    return input;
  }
}

int open_Comport(int cport_nr, int bdrate, char mode[4]){
  if(RS232_OpenComport(cport_nr, bdrate, mode))
  {
    cout << "Cannot open comport" << endl;
    return 1;
  }
  else {
    return 0;
  }
}

int add_entries(string potId, float nutLvl, float waterTemp, float waterLvl, 
		float waterCond, float ambientTemp, float ambientHumid, float lightIntensity)
{
  ostringstream executeString;
  cout << "Adding sensor data to database..." << endl;

  try {
	sql::Driver* driver = get_driver_instance();
	std::auto_ptr<sql::Connection> con(driver->connect(SQL_HOST, SQL_USER, SQL_PASS));
	con->setSchema(SQL_DB);
	std::auto_ptr<sql::Statement> stmt(con->createStatement());
	
	// We need not check the return value explicitly. If it indicates
	// an error, Connector/C++ generates an exception.

	executeString<<"CALL add_sensor_data("
		<< potId << ","
		<< nutLvl << ","
		<< waterTemp << ","
		<< waterLvl << ","
		<< waterCond << ","
		<< ambientTemp << ","
		<< ambientHumid << ","
		<< lightIntensity << ")";
	
	stmt->execute(executeString.str());
	executeString.clear();
  } catch (sql::SQLException &e) {
    /*
      MySQL Connector/C++ throws three different exceptions:

      - sql::MethodNotImplementedException (derived from sql::SQLException)
      - sql::InvalidArgumentException (derived from sql::SQLException)
      - sql::SQLException (derived from std::runtime_error)
    */
    cout << "# ERR: SQLException in " << __FILE__;
    cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
    /* what() (derived from std::runtime_error) fetches error message */
    cout << "# ERR: " << e.what();
    cout << " (MySQL error code: " << e.getErrorCode();
    cout << ", SQLState: " << e.getSQLState() << " )" << endl;

    return EXIT_FAILURE;
  }

  cout << "Done." << endl;
  return EXIT_SUCCESS;
}



int main(int argc, const char **argv)
{
  int x;
  int cport_nr=24,        /* /dev/ttyACM0 (COM? on Linux) */
      bdrate=9600;       /* 9600 baud */
  char mode[]={'8','N','1',0};
  float sensor_readings[8];
  string potId, inputString;

  open_Comport(cport_nr, bdrate, mode);
  
  usleep(100000); //short delay

  while(1)
  {
    inputString = check_Comport(cport_nr);
    x=0;
    istringstream ss(inputString);
    string token;

    while(getline(ss, token, ',')) {
      sensor_readings[x] = stof(token);
      cout << "Token "<< x << ": " << token <<endl;
      if (x==0){
        potId = token;
      }
      x++;
    }
    ss.clear();
    if(add_entries(potId,
      sensor_readings[4], // liquid level or nutrient level
      sensor_readings[2], // water temp
      sensor_readings[3], // water level
      sensor_readings[1], // water conductivity
      sensor_readings[5], // air temp
      sensor_readings[6], // air humidity
      sensor_readings[7])){ // light intensity
      cout << "Cannot write to SQL database!" << endl;
      return(0);
    }
  }
  return(0);
}

