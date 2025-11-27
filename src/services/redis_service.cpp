#include <string>
#include <iostream>
#include <exception>
#include <hiredis/hiredis.h>
#include "redis_service.h"

redisContext *c = nullptr;

redisContext *create_redis_connection(){
    try{
        c = redisConnect("127.0.0.1", 6379);
        if (c==NULL || c->err){
            if (c)
                std::cerr << c->errstr << std::endl;
            else
                std::cerr << "Can't allocate Redis context" << std::endl;
            return nullptr;
        }
        else
            std::cout << "Connected to Redis" << std::endl;
    
        return c;
    }
    catch(const std::exception &e){
        std::cerr << "Exception creating Redis connection: " << e.what() << std::endl;
        return nullptr;
    }
}

redisContext *get_redis_connection(){
    if (c!=nullptr){
        return c;
    }
    else{
        return create_redis_connection();
    }
}

void redis_service::set_value(const std::string &key, const std::string &value){
    redisContext *conn = get_redis_connection();
    if (!conn) {
        std::cerr << "No Redis connection available in set_value" << std::endl;
        return;
    }
    redisReply *reply = static_cast<redisReply*> (redisCommand(conn, "SET %s %s", key.c_str(), value.c_str()));
    if (reply) {
        freeReplyObject(reply);
    } else {
        std::cerr << "Redis SET command failed" << std::endl;
    }
}


std::string redis_service::get_value(const std::string &key){
    redisContext *conn = get_redis_connection();
    if (!conn) {
        std::cerr << "No Redis connection available in get_value" << std::endl;
        return std::string();
    }
    redisReply *reply = static_cast<redisReply*> (redisCommand(conn, "GET %s", key.c_str()));
    if (!reply) {
        std::cerr << "Redis GET command failed" << std::endl;
        return std::string();
    }

    std::string result;
    if (reply->type == REDIS_REPLY_STRING && reply->str) {
        result.assign(reply->str, reply->len);
    }
    freeReplyObject(reply);
    return result;
}