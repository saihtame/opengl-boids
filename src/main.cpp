#include "app/app.hpp"


int main() {
    ParticleSim::App::App application;

    if (!application.run())
        return 1;

    return 0;
}
