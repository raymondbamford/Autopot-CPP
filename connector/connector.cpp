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
#define RS232_PORTNR  38

using namespace std;


template<class T>
T check_null(float check_float);

string comports[RS232_PORTNR]={"/dev/ttyS0","/dev/ttyS1","/dev/ttyS2","/dev/ttyS3","/dev/ttyS4","/dev/ttyS5",
                       "/dev/ttyS6","/dev/ttyS7","/dev/ttyS8","/dev/ttyS9","/dev/ttyS10","/dev/ttyS11",
                       "/dev/ttyS12","/dev/ttyS13","/dev/ttyS14","/dev/ttyS15","/dev/ttyUSB0",
                       "/dev/ttyUSB1","/dev/ttyUSB2","/dev/ttyUSB3","/dev/ttyUSB4","/dev/ttyUSB5",
                       "/dev/ttyAMA0","/dev/ttyAMA1","/dev/ttyACM0","/dev/ttyACM1",
                       "/dev/rfcomm0","/dev/rfcomm1","/dev/ircomm0","/dev/ircomm1",
                       "/dev/cuau0","/dev/cuau1","/dev/cuau2","/dev/cuau3",
                       "/dev/cuaU0","/dev/cuaU1","/dev/cuaU2","/dev/cuaU3"};

int datacount=0;
float water_level_array[5];
void parse_string(string* input, string* potId, float sensor_readings[7]){
  int x=0;
  istringstream ss(*input);
  string token;

  while(getline(ss, token, ',')) {
    sensor_readings[x] = stof(token);
//  cout << "Token "<< x << ": " << token <<endl;
    if (x==0){
      *potId = token;
    }
    x++;
  }
  ss.clear();
}

int send_to_sql(float* sensor_readings[7], string* potId){

}

int check_arguments(int argc, const char** argv, int * cport_nr){
  /* /dev/ttyACM0 (COM? on Linux) */
  if (argc == 1) {
    cout << "Wrong number of arguments" << endl;
    return EXIT_FAILURE;
  } else {
    for(int i = 0; i < RS232_PORTNR; i++){
      if(string(argv[1]).compare(comports[i])==0){
        *cport_nr = i;
        cout << "Comport: " << argv[1] << "   Port number: " << *cport_nr << endl;
        return EXIT_SUCCESS;
      }
    }
  }
}

string check_Comport(int cport_nr){
  int n, total_bytes = 0;
  unsigned char buf[4096];
  stringstream ss;
  string input="", lastchar;
  bool serial_quiet = 0;
  
  n = RS232_PollComport(cport_nr, buf, 4095);

  if(n > 0){
    while(!serial_quiet){
      buf[n] = 0;   /* always put a "null" at the end of a string! */

      for(int i=0; i < n; i++){
        if(buf[i] < 32){  /* replace unreadable control-codes by dots */
          buf[i] = '.';
        }
      }
      ss << string((char*)buf);
      input = ss.str();
      cout << "Last char of string: " << input.back() << endl;
      total_bytes += n;
      
      lastchar = input.back();
      
      if(lastchar.compare(";")==0){
        cout << "received "<< total_bytes <<" bytes: " << input << endl;
        serial_quiet = 1;
      } else {
        usleep(100000); // wait 100ms to allow serial bus input to finish
        n = RS232_PollComport(cport_nr, buf, 4095);
      }
    }
    input = input.substr(0, input.size()-1);
    ss.clear();
    serial_quiet = 0;
  }
  return input;
}

int open_Comport(int cport_nr, int bdrate, char mode[4]){
  if(RS232_OpenComport(cport_nr, bdrate, mode))
  {
    cout << "Cannot open comport" << endl;
    return EXIT_FAILURE;
  }
  else {
    cout << "Comport connected!" << endl;
    return EXIT_SUCCESS;
  }
}

string check_null(float check_float){
  stringstream ss;
  if(check_float == -100){
    return "NULL";
  } else {
    ss << check_float;
    return ss.str();
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

  /* Check if air temp and air humidity are -100, and change them to NULL if they are. */


	executeString<<"CALL add_sensor_data("
		<< potId << ","
		<< nutLvl << ","
		<< waterTemp << ","
		<< waterLvl << ","
		<< waterCond << ","
		<< check_null(ambientTemp) << ","
		<< check_null(ambientHumid) << ","
		<< lightIntensity << ")";
	cout << "SQL command: " << executeString.str() << endl;
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

void water_level_mv_avg(float* water_level){
  float sum_water_level=0;
  if(datacount < 5){
    water_level_array[datacount] = *water_level;
    datacount++;
  } else {
    for(int  i = 0; i < 5; i++){
      sum_water_level += water_level_array[i];
    }
    if(abs(*water_level - sum_water_level/5) > 5){
      *water_level = (*water_level + sum_water_level)/6;
      cout << "Water level value changed to: " << *water_level << endl;
    }
    for(int i = 0; i < 4; i++){
      water_level_array[i] = water_level_array[i+1];
    }
    water_level_array[4] = *water_level;
  }
}

int main(int argc, const char **argv)
{
  int cport_nr, bdrate=9600; /* 9600 baud */
  char mode[]={'8','N','1',0};
  float sensor_readings[7];
  string potId, inputString;
  
  if(check_arguments(argc, argv, &cport_nr)) return EXIT_FAILURE;

  if(open_Comport(cport_nr, bdrate, mode)) return EXIT_FAILURE;
  usleep(1000000); // 1 second delay before beginning loop

  while(1) // super loop
  { 
    inputString = check_Comport(cport_nr); // check comport for input
    if(!(inputString.compare("")==0)){ // check if input is non-zero
      parse_string(&inputString, &potId, sensor_readings); // parse input string
      water_level_mv_avg(&sensor_readings[3]);
      
      /* hardcode air temp and air humidity */
      //sensor_readings[4] = 25.2;
      //sensor_readings[5] = 50.1;
      
      if(add_entries("'HAL'", // send data to SQL server
        0, // liquid level/nutrient level is always zero because the sensor is not being used
        sensor_readings[2], // water temp
        sensor_readings[3], // water level
        sensor_readings[1], // water conductivity
        sensor_readings[4], // air temp
        sensor_readings[5], // air humidity
        sensor_readings[6])){ // light intensity
        cout << "Cannot write to SQL database!" << endl;
        return EXIT_FAILURE;
      }
    }
  }
  return(0);
}

