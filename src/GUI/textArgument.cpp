/*
 * Copyright (C) 2024-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "baseGUI.hpp"

#if (USE_SDL_FONT) && (PRELOAD_FONTS)


SDL_Texture* GUI::TextArgument::createTexture(const Window& _window, const char* _text) const {
    // Getting font
    TTF_Font* fontData = _window.getFont(font);
    TTF_SetFontSize(fontData, height);

    // Creating hightlighted text
    TTF_SetFontOutline(fontData, 0);
    SDL_Surface* frontSurface = TTF_RenderText_Solid(fontData, _text, 0, textColor);

    // Check if has thickness
    if (frame) {
        // Create outline
        TTF_SetFontOutline(fontData, frame);
        SDL_Surface* surface = TTF_RenderText_Solid(fontData, _text, 0, {1, 0, 0, 255});

        // Merging surfaces
        SDL_SetSurfaceBlendMode(frontSurface, SDL_BLENDMODE_NONE);
        _window.setBlendMode(frontSurface);
        SDL_BlitSurface(frontSurface, nullptr, surface, nullptr);
        SDL_DestroySurface(frontSurface);

        // Creating texture from created surface
        return _window.createTextureAndFree(surface);
    }
    // Creating normal texture
    return _window.createTextureAndFree(frontSurface);
}

SDL_FRect GUI::TextArgument::getRect(const Window& _window, const SDL_Texture* _texture) const {
    SDL_FRect rect;
    rect.w = _texture->w;
    rect.h = _texture->h;
    rect.x = SDL_roundf(X * _window.getWidth() - rect.w * (unsigned)horAli/2);
    rect.y = SDL_roundf(Y * _window.getHeight() - rect.h * (unsigned)verAli/2);
    return rect;
}

SDL_FRect GUI::TextArgument::getRect(const Window& _window, float _W, float _H) const {
    SDL_FRect rect;
    rect.w = _W * _window.getWidth();
    rect.h = _H * _window.getHeight();
    rect.x = SDL_roundf(X * _window.getWidth() - rect.w * (unsigned)horAli/2);
    rect.y = SDL_roundf(Y * _window.getHeight() - rect.h * (unsigned)verAli/2);
    return rect;
}

#endif  // (USE_SDL_FONT) && (PRELOAD_FONTS)
