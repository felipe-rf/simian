#include "routes.hpp"

#include "../controllers/dna_controller.hpp"
#include "../controllers/stats_controller.hpp"
#include "../utils/file.hpp"
void register_dna_routes(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/simian")
    .methods("POST"_method)
    ([](const crow::request& req) {
        return check_dna(req);
    });
}

void register_stats_routes(crow::SimpleApp& app) {
    CROW_ROUTE(app, "/stats")
    .methods("GET"_method)
    ([]() {
        return get_stats();
    });
}

void register_docs_routes(crow::SimpleApp& app) {
    const std::string BASE = "public/swagger/";

    CROW_ROUTE(app, "/docs")
    .methods("GET"_method)
    ([BASE](const crow::request&, crow::response& res){
        res.set_header("Content-Type", "text/html");
        res.write(read_file(BASE + "index.html"));
        res.end();
    });

    CROW_ROUTE(app, "/openapi.yaml")
    .methods("GET"_method)
    ([BASE](const crow::request&, crow::response& res){
        res.set_header("Content-Type", "application/yaml");
        res.write(read_file(BASE + "openapi.yaml"));
        res.end();
    });
}