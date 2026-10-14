/*
 * Copyright (C) 2024-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "audio.hpp"
#include "../logger.hpp"

#if (USE_SDL_MIXER)

Audio::Audio()
: audioDeviceID(getAudioDeviceID()),
mixer(openMixer(audioDeviceID)),
#if (PRELOAD_MUSIC)
music(mixer),
#endif
#if (PRELOAD_SOUNDS)
sounds(mixer),
#endif
nc() {
    logger.additional("Audio created correctly");
}

Audio::~Audio() {
    // Destrying mixer and audio device
    MIX_DestroyMixer(mixer);
    SDL_CloseAudioDevice(audioDeviceID);

    // Closing audio library
    MIX_Quit();
}

SDL_AudioDeviceID Audio::getAudioDeviceID() {
    // Initialising audio library
    if (!MIX_Init()) {
        logger.important("Couldn't initialise mixer library: %s", SDL_GetError());
    }

    // Selecting audio device for audio output
    SDL_AudioDeviceID audioDeviceID = SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (audioDeviceID == 0) {
        logger.important("Couldn't get audio device ID: %s", SDL_GetError());
    }
    return audioDeviceID;
}

MIX_Mixer* Audio::openMixer(SDL_AudioDeviceID _devid) {
    // Creating mixer
    MIX_Mixer* mixer = MIX_CreateMixerDevice(_devid, nullptr);
    if (mixer == nullptr) {
        logger.important("Couldn't create mixer: %s", SDL_GetError());
    }
    return mixer;
}

void Audio::setVolume(float _volume) {
    MIX_SetMixerGain(mixer, _volume);
}

float Audio::getVolume() const {
    return MIX_GetMixerGain(mixer);
}

#endif  // (USE_SDL_MIXER)
