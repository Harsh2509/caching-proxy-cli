#ifndef CACHING_PROXY_UTILS
#define CACHING_PROXY_UTILS


#include <string>
#include <vector>
#include <optional>

struct CommandConfig {
    int port;
    std::string origin_url;
};

// Extract port and origin URL from command arguments
std::optional<CommandConfig> extract_config(const std::vector<std::string>& command_args);

#endif 