/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "baseGUI.hpp"

#if (USE_SDL_FONT) && (PRELOAD_FONTS)


GUI::SwitchBox::SwitchBox(const Window& _window, const TextArgument _arg, float _W,
    std::initializer_list<LanguagedText> _texts, unsigned _startOption) noexcept
: Template(_window),
selected(_startOption),
opened(false),
height(_arg.height*1.2f / _window.getHeight()),
backColor(_arg.backColor),
arrow(_window, _arg.X-_W/2+height/2, _arg.Y, height, _arg.textColor) {
    // Setting background
    background = _arg.getRect(window, _W, height);

    // Placing select options
    int i=0;
    for (const LanguagedText* text=_texts.begin(); text != _texts.end(); ++text) {
        drawnTexts.emplace_back(_window, std::move(*text), std::move(_arg));
        drawnTexts[i].move(0.0, height*i);
        i++;
    }
    // Placing selected text
    drawnTexts[selected].move(0.0, -height*selected);
}

GUI::SwitchBox::SwitchBox(SwitchBox&& _object) noexcept
: Template(std::move(_object)),
selected(_object.selected),
opened(_object.opened),
height(_object.height),
background(std::move(_object.background)),
backColor(_object.backColor),
drawnTexts(std::move(_object.drawnTexts)),
arrow(std::move(_object.arrow)) {}

void GUI::SwitchBox::set(unsigned _value) {
    if (opened) {
        selected = _value;
        opened = false;
        drawnTexts[selected].move(0.0, -height*selected);
        background.h = height * window.getHeight();
    } else {
        // Resetting old option
        drawnTexts[selected].move(0.0, height*selected);
        // Moving new option
        selected = _value;
        drawnTexts[selected].move(0.0, -height*selected);
    }
}

unsigned GUI::SwitchBox::getValue() const {
    return selected;
}

GUI::Code GUI::SwitchBox::click(const Mouse _mouse) {
    if (opened) {
        // Closing
        opened = false;
        // Selecting variant
        if (_mouse.in(background)) {
            // Finding new option
            selected = (_mouse.getY() - background.y) / (height * window.getHeight());
            drawnTexts[selected].move(0.0, -height*selected);
            background.h = height * window.getHeight();
            return Finished;
        }
        // Resetting to previous
        drawnTexts[selected].move(0.0, -height*selected);
        background.h = height * window.getHeight();
        return Some;
    } else {
        if (_mouse.in(background)) {
            // Selecting variant
            opened = true;
            background.h = height * drawnTexts.size() * window.getHeight();
            // Resetting selected postion
            drawnTexts[selected].move(0.0, height*selected);
            return Some;
        }
    }
    return None;
}

GUI::Code GUI::SwitchBox::press(SDL_Keycode _code) {
    if (opened && _code == SDLK_ESCAPE) {
        // Closing
        opened = false;
        drawnTexts[selected].move(0.0, -height*selected);
        background.h = height * window.getHeight();
        return Some;
    }
    return None;
}

void GUI::SwitchBox::move(float _X, float _Y) {
    background.x += _X*window.getWidth();
    background.y += _Y*window.getHeight();
    for (size_t i=0; i < drawnTexts.size(); ++i) {
        drawnTexts[i].move(_X, _Y);
    }
    arrow.move(_X, _Y);
}

void GUI::SwitchBox::blit() const {
    // Drawing background
    window.setDrawColor(backColor);
    window.drawRect(background);

    // Draw open or close version depend on state
    if (opened) {
        for (size_t i=0; i < drawnTexts.size(); ++i) {
            drawnTexts[i].blit();
        }
        arrow.blitFlipped();
    } else {
        drawnTexts[selected].blit();
        arrow.blit();
    }
}

#endif  // (USE_SDL_FONT) && (PRELOAD_FONTS)
