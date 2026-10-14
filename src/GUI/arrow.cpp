/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "baseGUI.hpp"


GUI::Arrow::Arrow(const Window& _window, float _X, float _Y, float _H, Color _color) noexcept
: Arrow(_window, {(_X-_H*0.4f)*_window.getWidth(), (_Y-_H*0.35f)*_window.getHeight(),
    _H*0.7f*_window.getHeight(), _H*0.7f*_window.getHeight()}, _color) {}

GUI::Arrow::Arrow(const Window& _window, const SDL_FRect& _rect, Color _color) noexcept
: TextureTemplate(_window, _rect, _window.createTexture(_rect.w, _rect.h)) {
    SDL_Vertex vertex[3] = {
        {  // Left point
            {0.0, 0.0},
            {0.0, 0.0, 0.0, 1.0},  // Black
        },
        {  // Down point
            {rect.h/2, rect.h},
            {0.0, 0.0, 0.0, 1.0},  // Black
        },
        {  // Right point
            {rect.h, 0.0},
            {0.0, 0.0, 0.0, 1.0},  // Black
        },
    };
    window.setDrawColor(_color);
    window.setRenderTarget(texture);
    window.drawGeometry(vertex, 3);
    window.resetRenderTarget();
}

GUI::Arrow::Arrow(const Arrow& _object) noexcept
: TextureTemplate(_object.window, _object.rect, _object.window.createTexture(_object.rect.w, _object.rect.h)) {
    window.copyTexture(texture, _object.texture);
}

GUI::Arrow::Arrow(Arrow&& _object) noexcept
: TextureTemplate(std::move(_object)) {
    texture = nullptr;
}

GUI::Arrow::~Arrow() noexcept {
    if (texture) {
        SDL_DestroyTexture(texture);
    }
}
