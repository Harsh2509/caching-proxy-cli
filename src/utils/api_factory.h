#ifndef CACHING_PROXY_API_FACTORY_H
#define CACHING_PROXY_API_FACTORY_H

#include <string>
#include <memory>

/**
 * @brief Result of an HTTP GET request
 */
struct HttpResponse {
    bool success;
    std::string body;
    std::string error_message;
    
    HttpResponse() : success(false) {}
};

/**
 * @brief Sends an HTTP GET request to the specified URL
 * 
 * @param url The URL to send the GET request to
 * @return HttpResponse containing the response body and status
 */
HttpResponse send_get_request(const std::string& url);

#endif // CACHING_PROXY_API_FACTORY_H
