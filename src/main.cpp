#include "app/app.hpp"


int main() {
    // Init application
    ParticleSim::App::App application;

    // Run
    if (!application.run())
        return 1;

    return 0;
}
