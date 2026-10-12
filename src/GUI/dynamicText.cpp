/*
 * Copyright (C) 2024-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "baseGUI.hpp"

#if (USE_SDL_FONT) && (PRELOAD_FONTS)


GUI::DynamicText::DynamicText(const Window& _window, const LanguagedText&& _texts,
    const TextArgument&& _arguments) noexcept
: TextureTemplate(_window),
texts(std::move(_texts)),
argument(std::move(_arguments)) {
    // Creating surface with text
    texture = argument.createTexture(window, texts);
    // Moving draw rect to new place
    rect = argument.getRect(window, texture);
}

GUI::DynamicText::DynamicText(DynamicText&& _object) noexcept
: TextureTemplate(std::move(_object)),
texts(std::move(_object.texts)),
argument(std::move(_object.argument)) {}

GUI::DynamicText::~DynamicText() noexcept {
    if (texture) {
        SDL_DestroyTexture(texture);
    } else {
        logger.important("Dynamic text error");
    }
}

#endif  // (USE_SDL_FONT) && (PRELOAD_FONTS)
