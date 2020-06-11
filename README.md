# libretro.js

An attempt to get a minimalistic [Emscripten](https://emscripten.org/)-based [libretro](https://www.libretro.com/) frontend working.

## Dependencies

- [Emscripten](https://emscripten.org/docs/getting_started/downloads.html)

## Build

```
git clone https://github.com/RobLoach/libretrojs.git
cd libretrojs
emmake make
```

This will build out to the `web` directory. You can start a server hosted on the web directory through Node.js by running...

```
emmake make start
```
