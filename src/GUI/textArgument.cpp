/*
 * Copyright (C) 2024-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "baseGUI.hpp"

#if (USE_SDL_FONT) && (PRELOAD_FONTS)


SDL_Texture* GUI::TextArgument::createTexture(const Window& window, const char* text) const {
    // Getting font
    TTF_Font* fontData = window.getFont(font);
    TTF_SetFontSize(fontData, height);

    // Creating hightlighted text
    TTF_SetFontOutline(fontData, 0);
    SDL_Surface* frontSurface = TTF_RenderText_Solid(fontData, text, 0, textColor);

    // Check if has thickness
    if (frame) {
        // Create outline
        TTF_SetFontOutline(fontData, frame);
        SDL_Surface* surface = TTF_RenderText_Solid(fontData, text, 0, {1, 0, 0, 255});

        // Merging surfaces
        SDL_SetSurfaceBlendMode(frontSurface, SDL_BLENDMODE_NONE);
        window.setBlendMode(frontSurface);
        SDL_BlitSurface(frontSurface, nullptr, surface, nullptr);
        SDL_DestroySurface(frontSurface);

        // Creating texture from created surface
        return window.createTextureAndFree(surface);
    }
    // Creating normal texture
    return window.createTextureAndFree(frontSurface);
}

SDL_FRect GUI::TextArgument::getRect(const Window& window, const SDL_Texture* texture) const {
    SDL_FRect rect;
    rect.w = texture->w;
    rect.h = texture->h;
    rect.x = SDL_roundf(window.getWidth() * X - rect.w * (unsigned)horAli/2);
    rect.y = SDL_roundf(window.getHeight() * Y - rect.h * (unsigned)verAli/2);
    return rect;
}

SDL_FRect GUI::TextArgument::getRect(const Window& window, float W, float H) const {
    SDL_FRect rect;
    rect.w = W;
    rect.h = H;
    rect.x = SDL_roundf(window.getWidth() * X - rect.w * (unsigned)horAli/2);
    rect.y = SDL_roundf(window.getHeight() * Y - rect.h * (unsigned)verAli/2);
    return rect;
}

#endif  // (USE_SDL_FONT) && (PRELOAD_FONTS)
