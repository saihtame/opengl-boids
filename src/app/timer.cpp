#include "timer.hpp"


namespace ParticleSim::App {

double Timer::get_delta() {
    // Delta
    uint64_t tick = SDL_GetPerformanceCounter();
    double delta = (double)(tick - last_tick) / frequency;
    last_tick = tick;
    return delta;
}

}
