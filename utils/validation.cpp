#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
#include "utils.h"
#include <regex>
#include <cctype>
#include "validation.h"

bool is_command_valid(const std::vector<std::string> &command_args)
{

    bool is_valid_command = true;

    // First check: Length of command should be 4 "--port 3245 --origin google.com"
    if (command_args.size() != 4)
    {
        is_valid_command = false;
    }

    // Second check: Sequence of command should be correct.
        for (std::size_t command_index = 0; command_index < command_args.size(); command_index += 2)
    {
        const std::string &command_arg = command_args[command_index];

        // After --port an interger should come
        if (command_arg == "--port" &&
           (command_index + 1 >= command_args.size() ||
           !is_number(command_args[command_index + 1])))
        {
            std::cerr << "Expected a number after --port\n";
            is_valid_command = false;
            break;
        }
        // After --origin a URL should come
        else if (command_arg == "--origin" &&
                (command_index + 1 >= command_args.size() ||
                !is_url(command_args[command_index+1])))
        {
            std::cerr << "Invalid URL after --origin\n";
            is_valid_command = false;
            break;
        }
    }
    return is_valid_command;
}

bool is_number(const std::string &str)
{
    if (str.empty())
        return false;
    return std::all_of(str.begin(), str.end(), [](unsigned char c)
                       { return std::isdigit(c); });
}

bool is_url(const std::string &str){
    // Simple, permissive URL regex: optional scheme, optional www, domain, optional path
    static const std::regex url_regex(R"(^(https?://)?(www\\.)?[A-Za-z0-9.-]+\.[A-Za-z]{2,6}(/.*)?$)", std::regex::icase);
    return std::regex_match(str, url_regex);
}