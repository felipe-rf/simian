#include "cache_service.hpp"

static std::unordered_map<std::string, bool> cache;

std::optional<bool> cache_get(const std::string& key) {
    if (cache.find(key) != cache.end()) {
        return cache[key];
    }
    return std::nullopt;
}

void cache_set(const std::string& key, bool value) {
    cache[key] = value;
}