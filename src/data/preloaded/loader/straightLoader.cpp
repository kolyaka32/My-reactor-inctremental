/*
 * Copyright (C) 2024-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "straightLoader.hpp"


// Check, if need to load data straight from files
#if (PRELOAD_DATA) && !(ARCHIEVE_LOADING)

#include "../../logger.hpp"


SDL_IOStream* StraightLoader::load(const char* _fileName) const {
    // Creating modified name
    char* fileName;
    SDL_asprintf(&fileName, "../%s", _fileName);

    // Openning file straight from system
    SDL_IOStream* data = SDL_IOFromFile(fileName, "r");

    // Clearing temporary name
    SDL_free(fileName);

    // Checking correction of loaded font
    #if (CHECK_CORRECTION)
    if (data == nullptr) {
        logger.important("Can't load straight: %s", _fileName);
        return nullptr;
    }
    #endif
    return data;
}

#endif  // (PRELOAD_DATA) && !(ARCHIEVE_LOADING)
