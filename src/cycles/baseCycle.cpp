/*
 * Copyright (C) 2025-2026, Kazankov Nikolay 
 * <nik.kazankov.05@mail.ru>
 */

#include "baseCycle.hpp"


BaseCycle::BaseCycle(Window& _window)
: CycleTemplate(_window),
settings(window) {}

bool BaseCycle::inputMouseDown() {
    if (settings.click(mouse)) {
        return true;
    }
    return false;
}

void BaseCycle::inputMouseUp() {
    settings.unClick();
}

bool BaseCycle::inputKeys(SDL_Keycode _key) {
    if (_key == SDLK_ESCAPE) {
        settings.toggle();
        return true;
    }
    return false;
}

bool BaseCycle::inputMouseWheel(float _wheelY) {
    return settings.scroll(mouse, _wheelY);
}

void BaseCycle::update() {
    settings.update();
}
