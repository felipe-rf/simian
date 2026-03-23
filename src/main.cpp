#include "app.hpp"

int main() {
    auto app = create_app();
    
    app.port(8080).multithreaded().run();
}