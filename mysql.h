#ifndef _MYSQL_H_ 
#define _MYSQL_H_
#include <jdbc/mysql_driver.h>
#include <jdbc/mysql_connection.h>
//#include "mysql.cc"
void sql_add(std::string  , std::string );
bool sql_check(std::string  , std::string passwd="@DEFAULT@");
void sql_addhistory(std::string,std::string,std::string,std::string);
void sql_addconnect(std::string connectptr);
std::string  sql_find_my_msg(std::string user);
//bool sql_check(std::string );
#endif