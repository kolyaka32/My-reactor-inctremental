/*
 * Copyright (C) 2024-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "baseGUI.hpp"

#if (USE_SDL_FONT) && (PRELOAD_FONTS)


GUI::TextButton::TextButton(const Window& _window, const LanguagedText&& _texts, const TextArgument&& _arguments)
: StaticText(_window, std::move(_texts), std::move(_arguments)),
backplate(_window, {rect.x-10, rect.y-1, rect.w+20, rect.h+4}, (rect.h+4)/2, 3) {}

GUI::TextButton::TextButton(TextButton&& _object) noexcept
: StaticText(std::move(_object)),
backplate(std::move(_object.backplate)) {}

void GUI::TextButton::move(float _X, float _Y) {
    StaticText::move(_X, _Y);
    backplate.move(_X, _Y);
}

void GUI::TextButton::blit() const {
    backplate.blit();
    window.blit(texture, rect);
}

#endif  // (USE_SDL_FONT) && (PRELOAD_FONTS)
