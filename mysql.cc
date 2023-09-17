<<<<<<< HEAD
// mysql
#include "mysql.h"

#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/mysql_connection.h>

#include "jdbc/mysql_driver.h"
#include "json/json.h"
void sql_addconnect(std::string connectptr) {
    try {
        sql::mysql::MySQL_Driver *driver;
        driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con;
        con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");
        sql::Statement *tool;
        tool = con->createStatement();
        std::string classmysql = "INSERT INTO users(username,password,time,connection) VALUES (?,?,NOW(),?)";
        sql::PreparedStatement *insertconnect = con->prepareStatement(classmysql);
        insertconnect->setString(4, connectptr);
        delete insertconnect;
        delete tool;
        delete con;
    } catch (sql::SQLException &e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
}
void sql_addhistory(std::string sender, std::string receiver, std::string message, std::string isread) {
    try {
        sql::mysql::MySQL_Driver *driver;
        driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con;
        con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");
        sql::Statement *tool;
        tool = con->createStatement();
        std::string classmysql = "INSERT INTO chat(content,isread, sender,receiver,time) VALUES (?,?,?,?,NOW())";
        sql::PreparedStatement *insertdata = con->prepareStatement(classmysql);
        insertdata->setString(1, message);
        insertdata->setString(2, isread);
        insertdata->setString(3, sender);
        insertdata->setString(4, receiver);
        insertdata->executeUpdate();
        delete insertdata;
        delete tool;
        delete con;
    } catch (sql::SQLException &e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
}
void sql_add(std::string username, std::string passwd) {
    try {
        sql::mysql::MySQL_Driver *driver;
        driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con;
        // con = driver->connect("tcp://localhost:3306", "root", "abc.123");
        // con = driver->connect("tcp://192.168.31.78:3306", "root", "abc.123");
        con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");
        sql::Statement *tool;
        tool = con->createStatement();
        std::string classmysql = "INSERT INTO users(username, password, createtime) VALUES (?, ?, NOW())";
        sql::PreparedStatement *ptool = con->prepareStatement(classmysql);
        ptool->setString(1, username);
        ptool->setString(2, passwd);
        ptool->executeUpdate();
        delete ptool;
        delete tool;
        delete con;
    } catch (sql::SQLException &e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
}
// bool sql_check(std::string user) {
//     bool result = false;
//     try {
//         sql::mysql::MySQL_Driver *driver;
//         driver = sql::mysql::get_mysql_driver_instance();
//         sql::Connection *con;
//         con = driver->connect("tcp://localhost:3306", "root", "abc.123");
//         con->setSchema("flypen");
//         std::string sql = "SELECT * FROM users WHERE username = ? LIMIT 1";
//         sql::PreparedStatement *prepStmt = con->prepareStatement(sql);
//         prepStmt->setString(1, user);
//         sql::ResultSet *res = prepStmt->executeQuery();

//         // 获取查询结果
//         if (!(res->next())) {
//             result = true;
//             // 提取所有列的值
//             std::string username = res->getString("username");
//             std::string password = res->getString("password");
//             int createtime = res->getInt("createtime");

//             // 在这里输出或使用提取的值
//             std::cout << "Username: " << username << std::endl;
//             std::cout << "Password: " << password << std::endl;
//             std::cout << "CreateTime: " << createtime << std::endl;
//         }

//         delete res;
//         delete prepStmt;
//         delete con;
//     } catch (sql::SQLException &e) {
//         //  std::cerr << "SQL Exception: " << e.what() << std::endl;
//     }

//     return result;
// }
Json::Value get_chat_info(std::string me,std::string who_send_me){
  Json::Value json;
  try {
    sql::mysql::MySQL_Driver *driver;
    driver = sql::mysql::get_mysql_driver_instance();
    sql::Connection *con;
    con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
    con->setSchema("flypen");

    std::string sql = "SELECT * FROM users WHERE username = ? LIMIT 1";
    sql::PreparedStatement *prepStmt = con->prepareStatement(sql);
    prepStmt->setString(1, who_send_me);
    
    sql::ResultSet *res = prepStmt->executeQuery();
    if (res->next()) {
      Json::Value user;
      int avatar=res->getInt("avatar");
      std::string friends=res->getString("friends");
      std::string req=res->getString("req");
      user["avatar"] = avatar;
      user["friends"] = friends; 
      user["req"] = req;
      
      json[who_send_me] = user;
    }
  } catch (sql::SQLException &e) {
    std::cerr << "SQL Exception: " << e.what() << std::endl;
  }
  return json;
}
bool sql_check(std::string user, std::string passwd) {
    bool result = false;
    try {
        sql::mysql::MySQL_Driver *driver;
        driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con;
        // con = driver->connect("tcp://127.0.0.1:3306", "root", "abc.123");
        // con = driver->connect("tcp://192.168.31.78:3306", "root", "abc.123");
        con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");
        std::string sql = "SELECT * FROM users WHERE username = ? LIMIT 1";
        sql::PreparedStatement *prepStmt = con->prepareStatement(sql);
        prepStmt->setString(1, user);
        sql::ResultSet *res = prepStmt->executeQuery();
        //    std::cout << passwd << std::endl;

        // 获取查询结果
        // (¬A ∧B) ∨ (A ∧¬B)
        if (((passwd != "@DEFAULT@") && (res->next())) || ((passwd == "@DEFAULT@") && (!(res->next())))) {
            result = true;
            // std::cout << "in" << std::endl;
            // 提取所有列的值
            std::string username = res->getString("username");
            std::string password = res->getString("password");
            int createtime = res->getInt("createtime");
            if ((passwd != password) && (passwd != "@DEFAULT@"))
                result = false;
            // 在这里输出或使用提取的值
            // std::cout << "SQL: Username: " << username << std::endl;
            // std::cout << "SQL: Password: " << password << std::endl;
            // std::cout << "SQL: CreateTime: " << createtime << std::endl;
        }

        delete res;
        delete prepStmt;
        delete con;
    } catch (sql::SQLException &e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
    std::cout << result << std::endl;

    return result;
}
Json::Value sql_find_my_msg(std::string me, std::string who_send_me) {
    // std::cout << "login user: " << user << std::endl;
    try {
        sql::mysql::MySQL_Driver *driver;
        driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con;
        con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");
        ////////////////////////////////////////////find begin
        //std::string sqlFind = "SELECT * FROM chat WHERE sender = ? AND receiver = ?";
        std::string sqlFind = "SELECT * FROM chat WHERE (sender = ? AND receiver = ?) OR (sender = ? AND receiver = ?)";
        sql::PreparedStatement *prepStmt = con->prepareStatement(sqlFind);
        prepStmt->setString(1, who_send_me);
        prepStmt->setString(2, me);
        prepStmt->setString(3, me);
        prepStmt->setString(4, who_send_me);
        std::cout << "sender: " << who_send_me << std::endl;
        std::cout << "receiver: " << me << std::endl;
        sql::ResultSet *res = prepStmt->executeQuery();
        ////////////////////////////////////////////find end and  update begin
        std::string sql0To1 = "UPDATE chat SET isread = 1 WHERE id = ?";
        sql::PreparedStatement *prepStmt0To1 = con->prepareStatement(sql0To1);
        int id;
        ///////////////////////////////////////////update end
        // Json::Value result;
        // std::map<std::string, std::vector<Json::Value>> grouped;

        // while (res->next()) {
        //     std::string sender = res->getString("sender");
        //     std::string content = res->getString("content");
        //     std::string time = res->getString("time");
        //     id = res->getInt("id");
        //     prepStmt0To1->setInt(1, id);
        //     sql::ResultSet *res0To1 = prepStmt0To1->executeQuery();
        //     Json::Value item;
        //     item["content"] = content;
        //     item["time"] = time;
        //     grouped[sender].push_back(item);
        // }

        // for (auto &[sender, items] : grouped) {
        //     Json::Value arr;
        //     for (auto &item : items) {
        //         arr.append(item);
        //     }
        //     Json::Value obj;
        //     obj[sender] = arr;
        //     result.append(obj);
        // }
        Json::Value result;
Json::Value messages;

while (res->next()) {

  std::string content = res->getString("content");
  std::string time = res->getString("time");
  std::string sender = res->getString("sender");

  Json::Value item;
  item["content"] = content; 
  item["time"] = time;
  item["sender"] = sender;

  messages.append(item);
}

result[who_send_me] = messages;
        delete res;
        delete prepStmt;
        delete con;
        return result;
    } catch (sql::SQLException &e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
        return "error";
    }
}
=======

// mysql
#include "mysql.h"

#include <jdbc/cppconn/prepared_statement.h>
#include <jdbc/mysql_connection.h>

#include "jdbc/mysql_driver.h"
#include "json/json.h"
void sql_addconnect(std::string connectptr) {
    try {
        sql::mysql::MySQL_Driver *driver;
        driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con;
        con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");
        sql::Statement *tool;
        tool = con->createStatement();
        std::string classmysql = "INSERT INTO users(username,password,time,connection) VALUES (?,?,NOW(),?)";
        sql::PreparedStatement *insertconnect = con->prepareStatement(classmysql);
        insertconnect->setString(4, connectptr);
        delete insertconnect;
        delete tool;
        delete con;
    } catch (sql::SQLException &e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
}
void sql_addhistory(std::string sender, std::string receiver, std::string message, std::string isread) {
    try {
        sql::mysql::MySQL_Driver *driver;
        driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con;
        con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");
        sql::Statement *tool;
        tool = con->createStatement();
        std::string classmysql = "INSERT INTO chat(content,isread, sender,receiver,time) VALUES (?,?,?,?,NOW())";
        sql::PreparedStatement *insertdata = con->prepareStatement(classmysql);
        insertdata->setString(1, message);
        insertdata->setString(2, isread);
        insertdata->setString(3, sender);
        insertdata->setString(4, receiver);
        insertdata->executeUpdate();
        delete insertdata;
        delete tool;
        delete con;
    } catch (sql::SQLException &e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
}
void sql_add(std::string username, std::string passwd, int avatar) {
    try {
        sql::mysql::MySQL_Driver *driver;
        driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con;
        // con = driver->connect("tcp://localhost:3306", "root", "abc.123");
        // con = driver->connect("tcp://192.168.31.78:3306", "root", "abc.123");
        con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");
        sql::Statement *tool;
        tool = con->createStatement();
        std::string classmysql = "INSERT INTO users(username, password, avatar, createtime) VALUES (?, ?, ?, NOW())";
        sql::PreparedStatement *ptool = con->prepareStatement(classmysql);
        ptool->setString(1, username);
        ptool->setString(2, passwd);
        ptool->setInt(3, avatar);   // 
        ptool->executeUpdate();
        delete ptool;
        delete tool;
        delete con;
    } catch (sql::SQLException &e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
}
// bool sql_check(std::string user) {
//     bool result = false;
//     try {
//         sql::mysql::MySQL_Driver *driver;
//         driver = sql::mysql::get_mysql_driver_instance();
//         sql::Connection *con;
//         con = driver->connect("tcp://localhost:3306", "root", "abc.123");
//         con->setSchema("flypen");
//         std::string sql = "SELECT * FROM users WHERE username = ? LIMIT 1";
//         sql::PreparedStatement *prepStmt = con->prepareStatement(sql);
//         prepStmt->setString(1, user);
//         sql::ResultSet *res = prepStmt->executeQuery();

//         // 获取查询结果
//         if (!(res->next())) {
//             result = true;
//             // 提取所有列的值
//             std::string username = res->getString("username");
//             std::string password = res->getString("password");
//             int createtime = res->getInt("createtime");

//             // 在这里输出或使用提取的值
//             std::cout << "Username: " << username << std::endl;
//             std::cout << "Password: " << password << std::endl;
//             std::cout << "CreateTime: " << createtime << std::endl;
//         }

//         delete res;
//         delete prepStmt;
//         delete con;
//     } catch (sql::SQLException &e) {
//         //  std::cerr << "SQL Exception: " << e.what() << std::endl;
//     }

//     return result;
// }
bool sql_check(std::string user, std::string passwd) {
    bool result = false;
    try {
        sql::mysql::MySQL_Driver *driver;
        driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con;
        // con = driver->connect("tcp://127.0.0.1:3306", "root", "abc.123");
        // con = driver->connect("tcp://192.168.31.78:3306", "root", "abc.123");
        con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");
        std::string sql = "SELECT * FROM users WHERE username = ? LIMIT 1";
        sql::PreparedStatement *prepStmt = con->prepareStatement(sql);
        prepStmt->setString(1, user);
        sql::ResultSet *res = prepStmt->executeQuery();
        //    std::cout << passwd << std::endl;

        // 获取查询结果
        // (¬A ∧B) ∨ (A ∧¬B)
        if (((passwd != "@DEFAULT@") && (res->next())) || ((passwd == "@DEFAULT@") && (!(res->next())))) {
            result = true;
            // std::cout << "in" << std::endl;
            // 提取所有列的值
            std::string username = res->getString("username");
            std::string password = res->getString("password");
            int createtime = res->getInt("createtime");
            int avatar = res->getInt("avatar");
            if ((passwd != password) && (passwd != "@DEFAULT@"))
                result = false;
            // 在这里输出或使用提取的值
            // std::cout << "SQL: Username: " << username << std::endl;
            // std::cout << "SQL: Password: " << password << std::endl;
            // std::cout << "SQL: CreateTime: " << createtime << std::endl;
            std::cout << "SQL: Avatar: " << avatar << std::endl;
        }

        delete res;
        delete prepStmt;
        delete con;
    } catch (sql::SQLException &e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
    }
    std::cout << result << std::endl;

    return result;
}
Json::Value sql_find_my_msg(std::string me, std::string who_send_me) {
    // std::cout << "login user: " << user << std::endl;
    try {
        sql::mysql::MySQL_Driver *driver;
        driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *con;
        con = driver->connect("tcp://8.130.48.157:3306", "root", "abc.123");
        con->setSchema("flypen");
        ////////////////////////////////////////////find begin
        //std::string sqlFind = "SELECT * FROM chat WHERE sender = ? AND receiver = ?";
        std::string sqlFind = "SELECT * FROM chat WHERE (sender = ? AND receiver = ?) OR (sender = ? AND receiver = ?)";
        sql::PreparedStatement *prepStmt = con->prepareStatement(sqlFind);
        prepStmt->setString(1, who_send_me);
        prepStmt->setString(2, me);
        prepStmt->setString(3, me);
        prepStmt->setString(4, who_send_me);
        std::cout << "sender: " << who_send_me << std::endl;
        std::cout << "receiver: " << me << std::endl;
        sql::ResultSet *res = prepStmt->executeQuery();
        ////////////////////////////////////////////find end and  update begin
        std::string sql0To1 = "UPDATE chat SET isread = 1 WHERE id = ?";
        sql::PreparedStatement *prepStmt0To1 = con->prepareStatement(sql0To1);
        int id;
        ///////////////////////////////////////////update end
        // Json::Value result;
        // std::map<std::string, std::vector<Json::Value>> grouped;

        // while (res->next()) {
        //     std::string sender = res->getString("sender");
        //     std::string content = res->getString("content");
        //     std::string time = res->getString("time");
        //     id = res->getInt("id");
        //     prepStmt0To1->setInt(1, id);
        //     sql::ResultSet *res0To1 = prepStmt0To1->executeQuery();
        //     Json::Value item;
        //     item["content"] = content;
        //     item["time"] = time;
        //     grouped[sender].push_back(item);
        // }

        // for (auto &[sender, items] : grouped) {
        //     Json::Value arr;
        //     for (auto &item : items) {
        //         arr.append(item);
        //     }
        //     Json::Value obj;
        //     obj[sender] = arr;
        //     result.append(obj);
        // }
        Json::Value result;
Json::Value messages;

while (res->next()) {

  std::string content = res->getString("content");
  std::string time = res->getString("time");
  std::string sender = res->getString("sender");

  Json::Value item;
  item["content"] = content; 
  item["time"] = time;
  item["sender"] = sender;

  messages.append(item);
}

result[who_send_me] = messages;
        delete res;
        delete prepStmt;
        delete con;
        return result;
    } catch (sql::SQLException &e) {
        std::cerr << "SQL Exception: " << e.what() << std::endl;
        return "error";
    }
}
>>>>>>> f57de4b287d009d8cea1c1611e6714868588b3b5
