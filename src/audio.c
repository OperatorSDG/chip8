#include "../include/audio.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

static SDL_AudioDeviceID audio_device = 0;
static bool audio_initialized = false;

static void audio_callback(void* userdata, Uint8* stream, int len) {
    static int phase = 0;
    for (int i = 0; i < len; i++) {
        stream[i] = (phase < 200) ? 0xff : 0x00;
        phase = (phase + 1) % 400;
    }
}

int audio_init(void) {
    if (audio_initialized) return 1;

    if (SDL_InitSubSystem(SDL_INIT_AUDIO) != 0) {
        fprintf(stderr, "SDL audio subsystem init failed: %s\n", SDL_GetError());
        return 0;
    }

    SDL_AudioSpec want = {0};
    want.freq = 44100;
    want.format = AUDIO_U8;
    want.channels = 1;
    want.samples = 512;
    want.callback = audio_callback;

    audio_device = SDL_OpenAudioDevice(NULL, 0, &want, NULL, 0);
    if (audio_device == 0) {
        fprintf(stderr, "SDL_OpenAudioDevice failed: %s\n", SDL_GetError());
        return 0;
    }
    audio_initialized = true;
    return 1;
}

void audio_beep_on(void) {
    if (audio_initialized) SDL_PauseAudioDevice(audio_device, 0);
}

void audio_beep_off(void) {
    if (audio_initialized) SDL_PauseAudioDevice(audio_device, 1);
}