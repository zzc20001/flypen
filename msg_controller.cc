#include "msg_controller.h"

#include <drogon/drogon.h>
#include <json/json.h>

#include "jwt_controller.h"
#include "mysql.h"
using namespace drogon;
void chat(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    auto body = req->getBody();
    Json::Value req_json, res_json;
    Json::Reader reader;
    std::string bodyStr(body);
    if (!reader.parse(bodyStr, req_json)) {
        callback(HttpResponse::newHttpResponse());
        return;
    }
    Json::FastWriter writer;
    std::string authHeader = req->getHeader("Authorization");
    if (authHeader.substr(0, 7) == "Bearer ") {
        std::string bearerToken = authHeader.substr(7);
        // 在此处使用Bearer Token进行身份验证
        try {
            std::string sender = jwtDecrypt(bearerToken);
            std::string content = req_json["content"].asString();
            std::string receiver = req_json["receiver"].asString();
            std::cout << "Connect success: " << sender << std::endl;
            sql_addhistory(sender, receiver, content, "0");
        } catch (const std::exception &e) {
            std::cerr << e.what() << '\n';
            std::cout << "Wrong token" << std::endl;
        }
    } else {
        // 连接没有Authorization头部Bearer Token
        std::cout << "No Authorization" << std::endl;
    }
    std::string msg = req_json["content"].asString();
    auto output = writer.write(res_json);
    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");
    res->setBody(output);
    callback(res);
}
void check(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback) {
    auto body = req->getBody();
    Json::Value req_json, res_json;
    Json::Reader reader;
    std::string bodyStr(body);
    if (!reader.parse(bodyStr, req_json)) {
        callback(HttpResponse::newHttpResponse());
        return;
    }
    std::string me, who_send_me;
    Json::FastWriter writer;
    std::string authHeader = req->getHeader("Authorization");
    if (authHeader.substr(0, 7) == "Bearer ") {
        std::string bearerToken = authHeader.substr(7);
        // 在此处使用Bearer Token进行身份验证
        try {
            me = jwtDecrypt(bearerToken);
        } catch (const std::exception &e) {
            std::cerr << e.what() << '\n';
            std::cout << "Wrong token" << std::endl;
        }
    } else {
        // 连接没有Authorization头部Bearer Token
        std::cout << "No Authorization" << std::endl;
    }

    auto res = HttpResponse::newHttpResponse();
    res->addHeader("Access-Control-Allow-Origin", "*");
    who_send_me = req_json["person"].asString();
    auto output = writer.write(sql_find_my_msg(me, who_send_me));
    res->setBody(output);
    callback(res);
}