#include <emscripten.h>
#include <cstdlib>
#include <iostream>

#include "vendor/libretro-common/include/libretro.h"

struct context
{
    int iteration;
};

void mainloop(void *arg)
{
    context* ctx = (context*)arg;
}

int main()
{
  retro_init();

    context ctx;
    int fps = 60;
    int simulate_infinite_loop = -1;
    emscripten_set_main_loop_arg(mainloop, &ctx, fps, simulate_infinite_loop);

    return EXIT_SUCCESS;
}
