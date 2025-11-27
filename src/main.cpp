#include <iostream>
#include <string>
#include <vector>
#include <chrono>
#include "utils/cli_out.h"
#include "utils/utils.h"
#include "utils/validation.h"

long long factorial(int n) {
    long long result = 1;
    for (int i = 2; i <= n; ++i) result *= i;
    return result;
}

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
        // Check whether both flags are present anywhere in the arguments
        bool has_port = false;
        bool has_origin = false;
        for (const auto &a : command_args) {
            if (a == "--port") has_port = true;
            if (a == "--origin") has_origin = true;
        }

        if (has_port && has_origin) {
        // If command has --port and --origin flag
        // Valid command: caching-proxy --port <number> --origin <url> OR --origin <url> --port <number>
            if (is_command_valid(command_args)){
                // TODO: start the proxy (not implemented yet)
            } else {
                std::cerr << "Invalid command\n";
                print_help();
                return 1;
            }
        } else {
            std::cerr << "Unknown command: " << command << "\n";
            print_help();
            return 1;
        }
    }
    return 0;
}