#include "cli_out.h"
#include <iostream>

void print_help(){
    std::cout << "Usage: caching_proxy [--help] [--version] [--port 234 --origin google.com]\n";
}

void print_version(){
    std::cout << "CLI Tool v1.0.0\n";
}