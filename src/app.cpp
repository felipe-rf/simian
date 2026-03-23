#include "app.hpp"

#include "routes/routes.hpp"

crow::SimpleApp create_app() {
    crow::SimpleApp app;

    register_dna_routes(app);
    register_stats_routes(app);
    register_docs_routes(app);
    return app;
}