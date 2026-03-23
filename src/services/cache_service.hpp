#pragma once
#include <unordered_map>
#include <string>
#include <optional>

std::optional<bool> cache_get(const std::string& key);
void cache_set(const std::string& key, bool value);