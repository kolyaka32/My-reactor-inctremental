/*
 * Copyright (C) 2025-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#pragma once

#include "cell.hpp"


// Prepaired unified texture for cell
class CellTexture {
private:
    const Window& window;
    SDL_Texture* texture;

public:
    CellTexture(const Window& window, float side, Color center, Color border);
    ~CellTexture();
    void blit(const SDL_FPoint p) const;
};
