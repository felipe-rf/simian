#include "dna_controller.hpp"

#include "../services/cache_service.hpp"
#include "../services/db_service.hpp"
#include "../services/dna_service.hpp"
#include "../utils/hash.hpp"

crow::response check_dna(const crow::request& req) {
    auto body = crow::json::load(req.body);

    if (!body || !body.has("dna")) {
        return crow::response(400);
    }

    std::vector<std::string> dna;
    for (const auto& row : body["dna"]) {
        dna.push_back(row.s());
    }

    std::string hash = hash_dna(dna);

    // 🔥 1. Check cache
    auto cached = cache_get(hash);
    if (cached.has_value()) {
        return crow::response(cached.value() ? 200 : 403);
    }

    // 🐘 2. Check DB
    auto db_result = find_dna(hash);
    if (db_result.has_value()) {
        cache_set(hash, db_result.value());
        return crow::response(db_result.value() ? 200 : 403);
    }

    // 🧬 3. Compute
    bool result = isSimian(dna);

    // 💾 4. Save
    insert_dna(hash, req.body, result);
    cache_set(hash, result);

    return crow::response(result ? 200 : 403);
}