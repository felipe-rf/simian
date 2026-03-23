#include "hash.hpp"

std::string hash_dna(const std::vector<std::string>& dna) {
    std::string combined;
    for (const auto& row : dna) {
        combined += row;
    }
    std::string hash = std::to_string(std::hash<std::string>{}(combined));
    return hash;
}