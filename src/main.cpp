#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "utils/cli_out.h"
#include "utils/utils.h"
#include "utils/validation.h"
#include "services/redis_service.h"
#include "utils/api_factory.h"


int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_help();
        return 1;
    }
    std::string command = argv[1];

    // Build a vector of command arguments from argv[1] .. argv[argc-1]
    std::vector<std::string> command_args;
    for (int i = 1; i < argc; ++i) command_args.emplace_back(argv[i]);

    if (command == "--help") {
        print_help();
    } else if (command == "--version") {
        print_version();
    } else {
        // Check if --origin flag is present (required)
        bool has_origin = false;
        for (const auto &a : command_args) {
            if (a == "--origin") has_origin = true;
        }

        if (!has_origin) {
            std::cerr << "Error: --origin flag is required\n";
            print_help();
            return 1;
        }

        // Validate command and extract configuration
        if (!is_command_valid(command_args)) {
            std::cerr << "Invalid command\n";
            print_help();
            return 1;
        }

        auto config = extract_config(command_args);
        if (!config.has_value()) {
            return 1;
        }
    
        // Getting the url ready
        std::string url = config->origin_url;
        if (config->port != -1){
            url += ":" + std::to_string(config->port);
        }
        
        std::string response = redis_service::get_value(url);
        if (response.length()){
            std::cout << "Cache hit" << std::endl;
            std::cout << response << std::endl;
        }
        else{
            std::cout << "Cache miss" << std::endl;
            HttpResponse res = send_get_request(url);
            redis_service::set_value(url, res.body, 120);
            std::cout << res.body << std::endl;
        }
    }
    return 0;
}