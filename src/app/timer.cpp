#include "timer.hpp"
#include <iostream>


namespace ParticleSim::App {

double Timer::get_delta() {
    // Delta
    uint64_t tick = SDL_GetPerformanceCounter();
    double delta = (double)(tick - last_tick) / frequency;
    last_tick = tick;

    // FPS
    fps_print_time += delta;
    frame_count++;
    if (fps_print_time >= fps_print_interval) {
        fps = frame_count / fps_print_interval;
        std::cout << "fps: " << fps << std::endl;
        fps_print_time = 0.0f;
        frame_count = 0;
    }
    return delta;
}

}
