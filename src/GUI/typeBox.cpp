/*
 * Copyright (C) 2024-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "baseGUI.hpp"

#if (USE_SDL_FONT) && (PRELOAD_FONTS)


GUI::TypeBox::TypeBox(const Window& _window, const TextArgument&& _arg,
    size_t _len, const char *_startText) noexcept
: TypeField(_window, std::move(_arg), _len, _startText),
backplate(_window, {_arg.X*_window.getWidth()-(13*_len-2)*(int)_arg.horAli/2-4,
    _arg.Y*_window.getHeight()-_arg.height*0.85f, 13.0f*_len+4, _arg.height*1.75f}, 2, _arg.backColor, _arg.textColor) {}

GUI::TypeBox::TypeBox(TypeBox&& _object) noexcept
: TypeField(std::move(_object)),
backplate(std::move(_object.backplate)) {}

void GUI::TypeBox::move(float _X, float _Y) {
    TypeField::move(_X, _Y);
    backplate.move(_X, _Y);
}

void GUI::TypeBox::blit() const {
    // Rendering background picture for better typing
    backplate.blit();

    // Rendering text
    TypeField::blit();
}

bool GUI::TypeBox::in(const Mouse _mouse) const {
    return backplate.in(_mouse);
}

#endif  // (USE_SDL_FONT) && (PRELOAD_FONTS)
