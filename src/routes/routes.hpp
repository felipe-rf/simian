#pragma once

#include "crow.h"

void register_dna_routes(crow::SimpleApp& app);
void register_stats_routes(crow::SimpleApp& app);
void register_docs_routes(crow::SimpleApp& app);