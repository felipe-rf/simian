#pragma once
#include <string>
#include <optional>

std::optional<bool> find_dna(const std::string& hash);
void insert_dna(const std::string& hash, const std::string& sequence, bool is_simian);

int count_simian();
int count_human();