#include "stats_controller.hpp"

#include "../services/db_service.hpp"

crow::response get_stats() {
    int simian = count_simian();
    int human = count_human();

    double ratio = human == 0 ? 0 : static_cast<double>(simian) / human;

    crow::json::wvalue res;
    res["count_mutant_dna"] = simian;
    res["count_human_dna"] = human;
    res["ratio"] = ratio;

    return crow::response(200, res);
}
