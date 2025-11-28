#ifndef CACHING_PROXY_VALIDATION
#define CACHING_PROXY_VALIDATION

#include <iostream>
#include <string>
#include <vector>

bool is_number(const std::string& str);
bool is_command_valid(const std::vector<std::string>& command_args);
bool is_url(const std::string &str);
bool has_origin_flag(const std::vector<std::string>& command_args);

#endif