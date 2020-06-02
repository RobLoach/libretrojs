
gambatte.js: libretro.bc vendor/gambatte-libretro/gambatte_libretro_emscripten.bc
	emcc vendor/gambatte-libretro/gambatte_libretro_emscripten.bc libretro.bc -o gambatte.js

vendor/gambatte-libretro/gambatte_libretro_emscripten.bc:
	platform=emscripten make -C vendor/gambatte-libretro

libretro.bc:
	em++ libretro.cpp -O2 -o libretro.bc

clean:
	rm vendor/gambatte-libretro/gambatte_libretro_emscripten.bc
	rm gambatte.js
	rm gambate.wasm
	rm libretro.js
	rm libretro.bc
	rm libretro.o
	rm libretro.wasm
