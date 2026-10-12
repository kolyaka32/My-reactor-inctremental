/*
 * Copyright (C) 2024-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "baseGUI.hpp"

#if (USE_SDL_FONT) && (PRELOAD_FONTS)


GUI::TwoOptionBox::TwoOptionBox(const Window& _window, float _X, float _Y, float _W, float _H,
    const LanguagedText&& _title, const LanguagedText&& _button1,
    const LanguagedText&& _button2) noexcept
: SubWindow(_window, _X, _Y, _W, _H),
title(_window, std::move(_title), {_X, _Y-_H*0.3f, .frame=1, .height=GUI::SubTitle}),
button1(_window, std::move(_button1), {_X, _Y+_H*0.08f, .frame=1}),
button2(_window, std::move(_button2), {_X, _Y+_H*0.32f, .frame=1}) {}

GUI::TwoOptionBox::TwoOptionBox(TwoOptionBox&& _object) noexcept
: SubWindow(std::move(_object)),
title(std::move(_object.title)),
button1(std::move(_object.button1)),
button2(std::move(_object.button2)) {}

GUI::Code GUI::TwoOptionBox::click(const Mouse _mouse) {
    if (active) {
        // Returning to menu
        if (button1.in(_mouse)) {
            return Button1;
        }
        if (button2.in(_mouse)) {
            return Button2;
        }
        return Some;
    }
    return None;
}

void GUI::TwoOptionBox::move(float _X, float _Y) {
    SubWindow::move(_X, _Y);
    title.move(_X, _Y);
    button1.move(_X, _Y);
    button2.move(_X, _Y);
}

void GUI::TwoOptionBox::blit() const {
    if (active) {
        background.blit();
        title.blit();
        button1.blit();
        button2.blit();
    }
}

#endif  // (USE_SDL_FONT) && (PRELOAD_FONTS)
