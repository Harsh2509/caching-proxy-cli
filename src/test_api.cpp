#include <iostream>
#include "utils/api_factory.h"

int main() {
    std::cout << "Testing HTTP GET request...\n\n";
    
    // Test with a simple URL
    HttpResponse response = send_get_request("http://httpbin.org/get");
    
    if (response.success) {
        std::cout << "✓ Request successful!\n";
        std::cout << "Response body (first 200 chars):\n";
        std::cout << response.body.substr(0, 200) << "...\n";
    } else {
        std::cout << "✗ Request failed!\n";
        std::cout << "Error: " << response.error_message << "\n";
    }
    
    return 0;
}
