
gambatte.js: vendor/gambatte-libretro/gambatte_libretro_emscripten.bc
	emcc vendor/gambatte-libretro/gambatte_libretro_emscripten.bc -s LINKABLE=1 -s EXPORT_ALL=1 -o gambatte.js

vendor/gambatte-libretro/gambatte_libretro_emscripten.bc:
	platform=emscripten emmake make -C vendor/gambatte-libretro

clean:
	rm vendor/gambatte-libretro/gambatte_libretro_emscripten.bc
	rm gambatte.js
	rm gambate.wasm
	rm libretro.js
	rm libretro.bc
	rm libretro.o
	rm libretro.wasm
