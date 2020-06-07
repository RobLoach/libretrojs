libretro.bc:
	emcc src/libretro.cpp -s WASM=1 -s LINKABLE=1 -o src/libretro.bc

libretro.js: libretro.bc gambatte_libretro_emscripten.bc
	emcc src/libretro.bc vendor/gambatte-libretro/gambatte_libretro_emscripten.bc -o web/libretro.js

all: web/core-gambatte.js web/core-nestopia.js


core-gambatte.js: gambatte_libretro_emscripten.bc
	emcc vendor/gambatte-libretro/gambatte_libretro_emscripten.bc -s WASM=1 -s LINKABLE=1 -s EXPORTED_FUNCTIONS='["_retro_init"]' -o web/core-gambatte.js

gambatte_libretro_emscripten.bc:
	platform=emscripten emmake make -C vendor/gambatte-libretro



core-nestopia.js: nestopia_libretro_emscripten.bc
	emcc vendor/nestopia/libretro/nestopia_libretro_emscripten.bc -s WASM=1 -s LINKABLE=1 -s EXPORTED_FUNCTIONS='["_retro_init"]' -o web/core-nestopia.js

nestopia_libretro_emscripten.bc:
	platform=emscripten emmake make -C vendor/nestopia/libretro


clean:
	rm vendor/gambatte-libretro/gambatte_libretro_emscripten.bc
	rm web/core-gambatte.js
	rm web/core-gambatte.wasm
	rm vendor/nestopia/nestopia_libretro_emscripten.bc
	rm web/core-nestopia.js
	rm web/core-nestopia.wasm
