#pragma once
#include <SDL3/SDL_timer.h>


namespace ParticleSim::App {

class Timer {
public:
    double get_delta();

private: // Delta time
    uint64_t last_tick = SDL_GetPerformanceCounter();
    double frequency = (double)SDL_GetPerformanceFrequency();

private: // FPS
    float fps = 0.0f;
    float fps_print_interval = 1.0f;
    float fps_print_time = 0.0f;
    int frame_count = 0;
};

}
