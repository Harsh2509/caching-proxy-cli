#include "api_factory.h"
#include <curl/curl.h>
#include <string>
#include <vector>
#include <memory>
#include <stdexcept>

namespace {
    /**
     * @brief RAII wrapper for CURL initialization
     * 
     * Ensures curl_global_init is called once and curl_global_cleanup
     * is called at program exit.
     */
    class CurlGlobalInit {
    public:
        CurlGlobalInit() {
            curl_global_init(CURL_GLOBAL_DEFAULT);
        }
        ~CurlGlobalInit() {
            curl_global_cleanup();
        }
        // Delete copy and move constructors/assignments
        CurlGlobalInit(const CurlGlobalInit&) = delete;
        CurlGlobalInit& operator=(const CurlGlobalInit&) = delete;
        CurlGlobalInit(CurlGlobalInit&&) = delete;
        CurlGlobalInit& operator=(CurlGlobalInit&&) = delete;
    };

    /**
     * @brief RAII wrapper for CURL easy handle
     */
    class CurlHandle {
    public:
        CurlHandle() : handle_(curl_easy_init()) {
            if (!handle_) {
                throw std::runtime_error("Failed to initialize CURL handle");
            }
        }
        
        ~CurlHandle() {
            if (handle_) {
                curl_easy_cleanup(handle_);
            }
        }
        
        // Delete copy constructor/assignment
        CurlHandle(const CurlHandle&) = delete;
        CurlHandle& operator=(const CurlHandle&) = delete;
        
        // Allow move operations
        CurlHandle(CurlHandle&& other) noexcept : handle_(other.handle_) {
            other.handle_ = nullptr;
        }
        
        CurlHandle& operator=(CurlHandle&& other) noexcept {
            if (this != &other) {
                if (handle_) {
                    curl_easy_cleanup(handle_);
                }
                handle_ = other.handle_;
                other.handle_ = nullptr;
            }
            return *this;
        }
        
        CURL* get() const { return handle_; }
        
    private:
        CURL* handle_;
    };

    /**
     * @brief Write callback for libcurl to store response data
     * 
     * This callback is invoked by libcurl when data is received from the server.
     * It appends the received data to a std::string buffer.
     * 
     * @param contents Pointer to the received data chunk
     * @param size Size of each element (typically 1 byte)
     * @param nmemb Number of elements
     * @param userp User-provided pointer (points to std::string in our case)
     * @return Number of bytes processed (size * nmemb on success, 0 on failure)
     */
    size_t write_callback(void* contents, size_t size, size_t nmemb, void* userp) {
        const size_t total_size = size * nmemb;
        auto* response_buffer = static_cast<std::string*>(userp);
        
        try {
            response_buffer->append(static_cast<char*>(contents), total_size);
        } catch (const std::bad_alloc&) {
            // Return 0 to signal error to libcurl
            return 0;
        }
        
        return total_size;
    }

    /**
     * @brief Ensure CURL is globally initialized (thread-safe singleton)
     */
    void ensure_curl_initialized() {
        static CurlGlobalInit curl_init;
        (void)curl_init; // Suppress unused variable warning
    }

} // anonymous namespace

HttpResponse send_get_request(const std::string& url) {
    HttpResponse response;
    
    // Ensure CURL is initialized
    ensure_curl_initialized();
    
    try {
        // Create CURL handle with RAII
        CurlHandle curl_handle;
        std::string response_body;
        
        // Configure CURL options
        curl_easy_setopt(curl_handle.get(), CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl_handle.get(), CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl_handle.get(), CURLOPT_WRITEDATA, &response_body);
        curl_easy_setopt(curl_handle.get(), CURLOPT_FOLLOWLOCATION, 1L);
        
        // Set reasonable timeouts
        curl_easy_setopt(curl_handle.get(), CURLOPT_TIMEOUT, 30L);
        curl_easy_setopt(curl_handle.get(), CURLOPT_CONNECTTIMEOUT, 10L);
        
        // Perform the request
        CURLcode res = curl_easy_perform(curl_handle.get());
        
        if (res == CURLE_OK) {
            response.success = true;
            response.body = std::move(response_body);
        } else {
            response.success = false;
            response.error_message = curl_easy_strerror(res);
        }
        
    } catch (const std::exception& e) {
        response.success = false;
        response.error_message = std::string("Exception: ") + e.what();
    }
    
    return response;
}
