WASM:=1

index.html: libretrojs.bc testsw_libretro_emscripten.bc
	em++ src/libretrojs.bc vendor/libretro-samples/video/software/rendering/testsw_libretro_emscripten.bc -o web/index.html

libretrojs.bc:
	em++ src/libretrojs.cpp -s EXPORT_ALL=1 -s LINKABLE=1 -s WASM=$(WASM) -s MAIN_MODULE=1 -o src/libretrojs.bc

testsw_libretro_emscripten.bc:
	platform=emscripten emmake make -C vendor/libretro-samples/video/software/rendering

start: index.html
	npm i
	npm start
