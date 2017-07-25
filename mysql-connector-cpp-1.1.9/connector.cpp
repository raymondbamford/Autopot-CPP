#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <unistd.h>
/* uncomment for applications that use vectors */
/*#include <vector>*/

#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include "rs232.h"

#define SQL_HOST "localhost"
#define SQL_USER "autopot"
#define SQL_PASS "basil"
#define SQL_DB "autopot"

using namespace std;

int add_entries(int argc, const char **argv)
{
//  string url(argc >= 2 ? argv[1] : EXAMPLE_HOST);
//  const string user(argc >= 3 ? argv[2] : EXAMPLE_USER);
//  const string pass(argc >= 4 ? argv[3] : EXAMPLE_PASS);
//  const string database(argc >= 5 ? argv[4] : EXAMPLE_DB);

//  cout << "Connector/C++ tutorial framework..." << endl;
  cout << "Connecting to autopot database..." << endl;
  cout << endl;

  try {
	sql::Driver* driver = get_driver_instance();
	std::auto_ptr<sql::Connection> con(driver->connect(url, user, pass));
	con->setSchema(database);
	std::auto_ptr<sql::Statement> stmt(con->createStatement());
	
	// We need not check the return value explicitly. If it indicates
	// an error, Connector/C++ generates an exception.
	stmt->execute("CALL add_sensor_data()");

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
