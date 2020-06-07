#include <emscripten.h>

#include "../vendor/libretro-common/include/libretro.h"


#include <iostream>

int main() {
  printf("Hello!\n");

  retro_init();
  return 0;
}
