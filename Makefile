
web/core-gambatte.js: vendor/gambatte-libretro/gambatte_libretro_emscripten.bc
	emcc vendor/gambatte-libretro/gambatte_libretro_emscripten.bc -s WASM=1 -s LINKABLE=1 -s EXPORTED_FUNCTIONS='["_retro_init"]' -o web/core-gambatte.js

vendor/gambatte-libretro/gambatte_libretro_emscripten.bc:
	platform=emscripten emmake make -C vendor/gambatte-libretro

clean:
	rm vendor/gambatte-libretro/gambatte_libretro_emscripten.bc
	rm web/core-gambatte.js
	rm web/core-gambatte.wasm
