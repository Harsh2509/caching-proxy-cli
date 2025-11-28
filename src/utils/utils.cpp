#include "utils.h"
#include <iostream>
#include <stdexcept>

std::optional<CommandConfig> extract_config(const std::vector<std::string>& command_args) {
    CommandConfig config;
    config.port = -1; // Default port
    bool origin_found = false;
    
    for (std::size_t i = 0; i < command_args.size(); ++i) {
        const std::string& arg = command_args[i];
        
        if (arg == "--port") {
            // Check if next argument exists
            if (i + 1 >= command_args.size()) {
                std::cerr << "Error: --port flag requires a value\n";
                return std::nullopt;
            }
            
            // Extract port number
            try {
                config.port = std::stoi(command_args[i + 1]);
                if (config.port <= 0 || config.port > 65535) {
                    std::cerr << "Error: Port must be between 1 and 65535\n";
                    return std::nullopt;
                }
            } catch (const std::exception& e) {
                std::cerr << "Error: Invalid port number\n";
                return std::nullopt;
            }
            ++i; // Skip the next argument since we've processed it
        }
        else if (arg == "--origin") {
            // Check if next argument exists
            if (i + 1 >= command_args.size()) {
                std::cerr << "Error: --origin flag requires a value\n";
                return std::nullopt;
            }
            
            // Extract origin URL
            config.origin_url = command_args[i + 1];
            origin_found = true;
            ++i; // Skip the next argument since we've processed it
        }
    }
    
    // Validate that at least --origin flag was found
    if (!origin_found) {
        std::cerr << "Error: --origin flag is required\n";
        return std::nullopt;
    }
    
    return config;
}