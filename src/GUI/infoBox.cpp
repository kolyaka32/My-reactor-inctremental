/*
 * Copyright (C) 2024-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "baseGUI.hpp"

#if (USE_SDL_FONT) && (PRELOAD_FONTS)


GUI::InfoBox::InfoBox(const Window& _window, const LanguagedText&& _texts,
    const TextArgument&& _arguments, unsigned _decay) noexcept
: StaticText(_window, std::move(_texts), std::move(_arguments)),
decayTime(_decay),
endTime(0) {
    // Resetting transperance
    SDL_SetTextureAlphaMod(texture, 0.0);
}

GUI::InfoBox::InfoBox(InfoBox&& _object) noexcept
: StaticText(std::move(_object)),
decayTime(_object.decayTime),
endTime(_object.endTime) {
    // First update to correct showing
    update();
}

void GUI::InfoBox::update() {
    if (endTime >= getTime()) {
        // Setting text to be less and less visible
        SDL_SetTextureAlphaModFloat(texture, (float)(endTime - getTime()) / decayTime);
    } else {
        // Fully transperent
        SDL_SetTextureAlphaModFloat(texture, 0.0);
    }
}

void GUI::InfoBox::reset() {
    endTime = getTime() + decayTime;
}

#endif  // (USE_SDL_FONT) && (PRELOAD_FONTS)
