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
  int i, n, x,
      cport_nr=24,        /* /dev/ttyACM0 (COM? on Linux) */
      bdrate=9600;       /* 9600 baud */

  unsigned char buf[4096];
  float sensor_readings[8];
  string potId;
  char mode[]={'8','N','1',0};

  if(RS232_OpenComport(cport_nr, bdrate, mode))
  {
    cout << "Cannot open comport" << endl;

    return(0);
  }
  
  RS232_cputs(cport_nr, "a");
  usleep(100000); //short delay
  printf("Starting Arduino...\n");

  while(1)
  {
    x=0;
    n = RS232_PollComport(cport_nr, buf, 4095);

    if(n > 0)
    {
      buf[n] = 0;   /* always put a "null" at the end of a string! */

      for(i=0; i < n; i++)
      {
        if(buf[i] < 32)  /* replace unreadable control-codes by dots */
        {
          buf[i] = '.';
        }
      }
      string input((char *)buf);
      istringstream ss(input);
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
      printf("received %i bytes: %s\n", n, (char *)buf);
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

    usleep(4900000); // sleep for 4.9 seconds
    RS232_cputs(cport_nr, "a");
    usleep(100000); // short delay
  }

  return(0);
}

