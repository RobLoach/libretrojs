#include <cstdarg>
#include <emscripten.h>
#include <string>
#include <iostream>
#include <emscripten/html5.h>
#include "../vendor/libretro-common/include/libretro.h"

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

static bool core_environment(unsigned cmd, void *data) {
	switch (cmd) {
		case RETRO_ENVIRONMENT_GET_LOG_INTERFACE: {
			struct retro_log_callback *cb = (struct retro_log_callback *)data;
			cb->log = core_log;
			return true;
        }

		case RETRO_ENVIRONMENT_SET_PIXEL_FORMAT: {
			// TODO: Figure out the pixel formating.
			return true;
        }

		case RETRO_ENVIRONMENT_SET_SUPPORT_NO_GAME: {
			// TODO: Act on no game.
			return true;
        }
	}

    core_log(RETRO_LOG_DEBUG, "Unhandled env #%u", cmd);
    return false;
}

void core_video_refresh(const void *data, unsigned width, unsigned height, size_t pitch) {
	//if (data)
	//	video_refresh(data, width, height, pitch);
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
	/*int written = snd_pcm_writei(g_pcm, buf, frames);

	if (written < 0) {
		printf("Alsa warning/error #%i: ", -written);
		snd_pcm_recover(g_pcm, written, 0);

		return 0;
	}

	return written;
    */
    return 0;
}

static void core_audio_sample(int16_t left, int16_t right) {
	int16_t buf[2] = {left, right};
	audio_write(buf, 1);
}


static size_t core_audio_sample_batch(const int16_t *data, size_t frames) {
	return audio_write(data, frames);
}

void init_video(int width, int height) {
    double dpr = emscripten_get_device_pixel_ratio();
    emscripten_set_element_css_size("canvas", width / dpr, height / dpr);
    emscripten_set_canvas_element_size("canvas", width, height);
}

int main() {
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

    struct retro_system_av_info info = {0};
    retro_get_system_av_info(&info);

    int width = info.geometry.base_width;
    int height = info.geometry.base_height;
    init_video(width, height);
    std::cout << "Initialized " << system.library_name << std::endl;

    return 0;
}
