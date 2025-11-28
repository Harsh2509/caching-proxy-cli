#ifndef CACHING_PROXY_REDIS_SERVICE
#define CACHING_PROXY_REDIS_SERVICE

#include <string>
#include <iostream>

namespace redis_service {
    void set_value(const std::string& key, const std::string& value, const int &expire_seconds);
    std::string get_value(const std::string& key);
} // namespace redis_service

#endif
