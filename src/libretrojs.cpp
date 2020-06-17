#include <cstdarg>
#include <emscripten.h>
#include <string>
#include <iostream>
#include <emscripten/html5.h>
#include "../vendor/libretro-common/include/libretro.h"

class Context {
public:
    Context() {
        target = "canvas";
    };
    std::string target;

    int width;
    int height;
    int pitch;
    retro_pixel_format pixelFormat;
    bool supportsNoGame;


    uint32_t* screen;
    uint32_t texture[320*320];

    float GetPixelWidth() {
        return 1;
    }
    float GetPixelHeight() {
        return 1;
    }
};

Context* context;

static void core_log(enum retro_log_level level, const char *fmt, ...) {
	char buffer[4096] = {0};
	va_list va;

	va_start(va, fmt);
	vsnprintf(buffer, sizeof(buffer), fmt, va);
	va_end(va);

    switch ((int)level) {
        case RETRO_LOG_DEBUG:
        case RETRO_LOG_INFO:
            emscripten_console_log(buffer);
            break;
        case RETRO_LOG_WARN:
            emscripten_console_warn(buffer);
            break;
        case RETRO_LOG_ERROR:
            emscripten_console_error(buffer);
            break;
    }
}

bool setPixelFormat(Context* context, retro_pixel_format format) {
    context->pixelFormat = format;
    switch (format) {
    case RETRO_PIXEL_FORMAT_0RGB1555:
        //context->glPixelFormat = GL_UNSIGNED_SHORT_5_5_5_1;
        //context->glPixelType =  GL_RGBA; //GL_BGRA;
        //context->bpp = sizeof(uint16_t);
        std::cout << "Pixel ormat: RETRO_PIXEL_FORMAT_0RGB1555" << std::endl;
        break;
    case RETRO_PIXEL_FORMAT_XRGB8888:
        //context->glPixelFormat =  GL_UNSIGNED_SHORT_4_4_4_4; // GL_UNSIGNED_INT_8_8_8_8_REV;
        //context->glPixelType =  GL_RGBA; // GL_BGRA;
        //context->bpp = sizeof(uint32_t);
        std::cout << "Pixel ormat: RETRO_PIXEL_FORMAT_XRGB8888" << std::endl;
        break;
    case RETRO_PIXEL_FORMAT_RGB565:
        //context->glPixelFormat = GL_UNSIGNED_SHORT_5_6_5;
        //context->glPixelType = GL_RGB;
        //context->bpp = sizeof(uint16_t);
        std::cout << "Pixel ormat: RETRO_PIXEL_FORMAT_RGB565" << std::endl;
        break;
    default:
        return false;
    }

    return true;
}

const char* GetEnvironmentName(int environmentCallback) {
    switch (environmentCallback) {
        case RETRO_ENVIRONMENT_SET_ROTATION: return "RETRO_ENVIRONMENT_SET_ROTATION";
        case RETRO_ENVIRONMENT_GET_OVERSCAN: return "RETRO_ENVIRONMENT_GET_OVERSCAN";
        case RETRO_ENVIRONMENT_GET_CAN_DUPE: return "RETRO_ENVIRONMENT_GET_CAN_DUPE";
        case RETRO_ENVIRONMENT_SET_MESSAGE: return "RETRO_ENVIRONMENT_SET_MESSAGE";
        case RETRO_ENVIRONMENT_SHUTDOWN: return "RETRO_ENVIRONMENT_SHUTDOWN";
        case RETRO_ENVIRONMENT_SET_PERFORMANCE_LEVEL: return "RETRO_ENVIRONMENT_SET_PERFORMANCE_LEVEL";
        case RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY: return "RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY";
        case RETRO_ENVIRONMENT_SET_PIXEL_FORMAT: return "RETRO_ENVIRONMENT_SET_PIXEL_FORMAT";
        case RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS: return "RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS";
        case RETRO_ENVIRONMENT_SET_KEYBOARD_CALLBACK: return "RETRO_ENVIRONMENT_SET_KEYBOARD_CALLBACK";
        case RETRO_ENVIRONMENT_SET_DISK_CONTROL_INTERFACE: return "RETRO_ENVIRONMENT_SET_DISK_CONTROL_INTERFACE";
        case RETRO_ENVIRONMENT_SET_HW_RENDER: return "RETRO_ENVIRONMENT_SET_HW_RENDER";
        case RETRO_ENVIRONMENT_GET_VARIABLE: return "RETRO_ENVIRONMENT_GET_VARIABLE";
        case RETRO_ENVIRONMENT_SET_VARIABLES: return "RETRO_ENVIRONMENT_SET_VARIABLES";
        case RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE: return "RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE";
        case RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME: return "RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME";
        case RETRO_ENVIRONMENT_GET_LIBRETRO_PATH: return "RETRO_ENVIRONMENT_GET_LIBRETRO_PATH";
        case RETRO_ENVIRONMENT_SET_FRAME_TIME_CALLBACK: return "RETRO_ENVIRONMENT_SET_FRAME_TIME_CALLBACK";
        case RETRO_ENVIRONMENT_SET_AUDIO_CALLBACK: return "RETRO_ENVIRONMENT_SET_AUDIO_CALLBACK";
        case RETRO_ENVIRONMENT_GET_RUMBLE_INTERFACE: return "RETRO_ENVIRONMENT_GET_RUMBLE_INTERFACE";
        case RETRO_ENVIRONMENT_GET_INPUT_DEVICE_CAPABILITIES: return "RETRO_ENVIRONMENT_GET_INPUT_DEVICE_CAPABILITIES";
        case RETRO_ENVIRONMENT_GET_SENSOR_INTERFACE: return "RETRO_ENVIRONMENT_GET_SENSOR_INTERFACE";
        case RETRO_ENVIRONMENT_GET_CAMERA_INTERFACE: return "RETRO_ENVIRONMENT_GET_CAMERA_INTERFACE";
        case RETRO_ENVIRONMENT_GET_LOG_INTERFACE: return "RETRO_ENVIRONMENT_GET_LOG_INTERFACE";
        case RETRO_ENVIRONMENT_GET_PERF_INTERFACE: return "RETRO_ENVIRONMENT_GET_PERF_INTERFACE";
        case RETRO_ENVIRONMENT_GET_LOCATION_INTERFACE: return "RETRO_ENVIRONMENT_GET_LOCATION_INTERFACE";
        case RETRO_ENVIRONMENT_GET_CONTENT_DIRECTORY: return "RETRO_ENVIRONMENT_GET_CONTENT_DIRECTORY";
        // case RETRO_ENVIRONMENT_GET_CORE_ASSETS_DIRECTORY: return "RETRO_ENVIRONMENT_GET_CORE_ASSETS_DIRECTORY";
        case RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY: return "RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY";
        case RETRO_ENVIRONMENT_SET_SYSTEM_AV_INFO: return "RETRO_ENVIRONMENT_SET_SYSTEM_AV_INFO";
        case RETRO_ENVIRONMENT_SET_PROC_ADDRESS_CALLBACK: return "RETRO_ENVIRONMENT_SET_PROC_ADDRESS_CALLBACK";
        case RETRO_ENVIRONMENT_SET_SUBSYSTEM_INFO: return "RETRO_ENVIRONMENT_SET_SUBSYSTEM_INFO";
        case RETRO_ENVIRONMENT_SET_CONTROLLER_INFO: return "RETRO_ENVIRONMENT_SET_CONTROLLER_INFO";
        case RETRO_ENVIRONMENT_SET_MEMORY_MAPS: return "RETRO_ENVIRONMENT_SET_MEMORY_MAPS";
        case RETRO_ENVIRONMENT_SET_GEOMETRY: return "RETRO_ENVIRONMENT_SET_GEOMETRY";
        case RETRO_ENVIRONMENT_GET_USERNAME: return "RETRO_ENVIRONMENT_GET_USERNAME";
        case RETRO_ENVIRONMENT_GET_LANGUAGE: return "RETRO_ENVIRONMENT_GET_LANGUAGE";
        case RETRO_ENVIRONMENT_GET_CURRENT_SOFTWARE_FRAMEBUFFER: return "RETRO_ENVIRONMENT_GET_CURRENT_SOFTWARE_FRAMEBUFFER";
        case RETRO_ENVIRONMENT_GET_HW_RENDER_INTERFACE: return "RETRO_ENVIRONMENT_GET_HW_RENDER_INTERFACE";
        case RETRO_ENVIRONMENT_SET_SUPPORT_ACHIEVEMENTS: return "RETRO_ENVIRONMENT_SET_SUPPORT_ACHIEVEMENTS";
        case RETRO_ENVIRONMENT_SET_HW_RENDER_CONTEXT_NEGOTIATION_INTERFACE: return "RETRO_ENVIRONMENT_SET_HW_RENDER_CONTEXT_NEGOTIATION_INTERFACE";
        case RETRO_ENVIRONMENT_SET_SERIALIZATION_QUIRKS: return "RETRO_ENVIRONMENT_SET_SERIALIZATION_QUIRKS";
        case RETRO_ENVIRONMENT_SET_HW_SHARED_CONTEXT: return "RETRO_ENVIRONMENT_SET_HW_SHARED_CONTEXT";
        case RETRO_ENVIRONMENT_GET_VFS_INTERFACE: return "RETRO_ENVIRONMENT_GET_VFS_INTERFACE";
        case RETRO_ENVIRONMENT_GET_LED_INTERFACE: return "RETRO_ENVIRONMENT_GET_LED_INTERFACE";
        case RETRO_ENVIRONMENT_GET_AUDIO_VIDEO_ENABLE: return "RETRO_ENVIRONMENT_GET_AUDIO_VIDEO_ENABLE";
        case RETRO_ENVIRONMENT_GET_MIDI_INTERFACE: return "RETRO_ENVIRONMENT_GET_MIDI_INTERFACE";
        case RETRO_ENVIRONMENT_GET_FASTFORWARDING: return "RETRO_ENVIRONMENT_GET_FASTFORWARDING";
        case RETRO_ENVIRONMENT_GET_TARGET_REFRESH_RATE: return "RETRO_ENVIRONMENT_GET_TARGET_REFRESH_RATE";
        case RETRO_ENVIRONMENT_GET_INPUT_BITMASKS: return "RETRO_ENVIRONMENT_GET_INPUT_BITMASKS";
        case RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION: return "RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION";
        case RETRO_ENVIRONMENT_SET_CORE_OPTIONS: return "RETRO_ENVIRONMENT_SET_CORE_OPTIONS";
        case RETRO_ENVIRONMENT_SET_CORE_OPTIONS_INTL: return "RETRO_ENVIRONMENT_SET_CORE_OPTIONS_INTL";
        case RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY: return "RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY";
        case RETRO_ENVIRONMENT_GET_PREFERRED_HW_RENDER: return "RETRO_ENVIRONMENT_GET_PREFERRED_HW_RENDER";
        case RETRO_ENVIRONMENT_GET_DISK_CONTROL_INTERFACE_VERSION: return "RETRO_ENVIRONMENT_GET_DISK_CONTROL_INTERFACE_VERSION";
        case RETRO_ENVIRONMENT_SET_DISK_CONTROL_EXT_INTERFACE: return "RETRO_ENVIRONMENT_SET_DISK_CONTROL_EXT_INTERFACE";
        case RETRO_ENVIRONMENT_GET_MESSAGE_INTERFACE_VERSION: return "RETRO_ENVIRONMENT_GET_MESSAGE_INTERFACE_VERSION";
        case RETRO_ENVIRONMENT_SET_MESSAGE_EXT: return "RETRO_ENVIRONMENT_SET_MESSAGE_EXT";
    }
    return "Undefined";
}

static bool core_environment(unsigned cmd, void *data) {
	switch (cmd) {
		case RETRO_ENVIRONMENT_GET_LOG_INTERFACE: {
			struct retro_log_callback *cb = (struct retro_log_callback *)data;
			cb->log = core_log;
			return true;
        }

		case RETRO_ENVIRONMENT_SET_PIXEL_FORMAT: {
			// TODO: Figure out the pixel formating.
            retro_pixel_format* pixelFormat = (retro_pixel_format*)data;
            setPixelFormat(context, *pixelFormat);
			return true;
        }

		case RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME: {
            bool* noGame = (bool*)data;
            context->supportsNoGame = *noGame;
			return true;
        }

        case RETRO_ENVIRONMENT_SHUTDOWN: {
            emscripten_cancel_main_loop();
            return true;
        }

        case RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE: {
            bool *bval = (bool*)data;
            *bval = false;
            return true;
        }
	}

    core_log(RETRO_LOG_WARN, "Unhandled env #%u %s", cmd, GetEnvironmentName(cmd));
    return false;
}


void core_video_render_canvas(uint32_t* ptr, int w, int h) {
  EM_ASM_({
      let data = Module.HEAPU8.slice($0, $0 + $1 * $2 * 4);
      let context = Module['canvas'].getContext('2d');
      let imageData = context.getImageData(0, 0, $1, $2);
      imageData.data.set(data);
      context.putImageData(imageData, 0, 0);
    }, ptr, w, h);
}

void core_video_convert_xrgb8888_to_rgbx8888(const void *data, unsigned width, unsigned height) {
    char* pixels = (char*)data;
    for (int i = 0; i < width * height * 4; i += 4) {
        pixels[i] = pixels[i+1];
        pixels[i+1] = pixels[i+2];
        pixels[i+2] = pixels[i+3];
        pixels[i+3] = 0xff;
    }
}

void core_video_refresh(const void *data, unsigned width, unsigned height, size_t pitch) {
    if (!data) {
        std::cout << "No data given" << std::endl;
        return;
    }

    context->screen = (uint32_t*)data;
    context->width = width;
    context->height = height;
    context->pitch = pitch;

    // Convert the pixels based on the pixel format.
    switch ((int)context->pixelFormat) {
        case RETRO_PIXEL_FORMAT_XRGB8888:
            core_video_convert_xrgb8888_to_rgbx8888(data, width, height);
            break;
    }

    // Render to the canvas.
    core_video_render_canvas(context->screen, width, height);
}

static void core_input_poll(void) {
    /*
	int i;
	for (i = 0; g_binds[i].k || g_binds[i].rk; ++i)
		g_joy[g_binds[i].rk] = (glfwGetKey(g_win, g_binds[i].k) == GLFW_PRESS);

	// Quit nanoarch when pressing the Escape key.
	if (glfwGetKey(g_win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(g_win, true);
	}*/
}

static int16_t core_input_state(unsigned port, unsigned device, unsigned index, unsigned id) {
	return 0;
}

static size_t audio_write(const void *buf, unsigned frames) {
    // TODO: Add OpenAL https://emscripten.org/docs/porting/Audio.html
    return 0;
}

static void core_audio_sample(int16_t left, int16_t right) {
	int16_t buf[2] = {left, right};
	audio_write(buf, 1);
}


static size_t core_audio_sample_batch(const int16_t *data, size_t frames) {
	return audio_write(data, frames);
}

void core_run(void* userArgs) {

    retro_run();


}

/**
 * Returns the program, or -1 on failure.
 */
bool init_video(Context* context) {
    int width = context->width;
    int height = context->height;
    std::cout << "Target: " << context->target.c_str() << std::endl;
  emscripten_set_canvas_element_size(context->target.c_str(), width, height);

  // TODO: Set up any other video
  return true;
}

int main() {
    context = new Context();
    // Verify API
    if (retro_api_version() != 1) {
        core_log(RETRO_LOG_ERROR, "retro_api_version isn't 1");
        return 1;
    }

    // Set up the environment.
    retro_set_environment(core_environment);
    retro_set_video_refresh(core_video_refresh);
    retro_set_input_poll(core_input_poll);
    retro_set_input_state(core_input_state);
    retro_set_audio_sample(core_audio_sample);
    retro_set_audio_sample_batch(core_audio_sample_batch);

    // Initialize libretro.
    retro_init();

    // System information.
    struct retro_system_info system = {0};
    retro_get_system_info(&system);
    core_log(RETRO_LOG_INFO, "Library: %s@%s", system.library_name, system.library_version);

    // Load game.
    if (!retro_load_game(NULL)) {
        core_log(RETRO_LOG_ERROR, "Failed to load game.");
        return 1;
    }

    // Core information.
    struct retro_system_av_info info = {0};
    retro_get_system_av_info(&info);

    context->width = info.geometry.base_width;
    context->height = info.geometry.base_height;
    init_video(context);
    std::cout << "Initialized " << system.library_name << std::endl;
    std::cout << "FPS: " << info.timing.fps << std::endl;
    emscripten_set_main_loop_arg(core_run, context, info.timing.fps, 0);

    return 0;
}
