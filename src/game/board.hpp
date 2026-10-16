/*
 * Copyright (C) 2025-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#pragma once

#include "cell.hpp"
#include "channel.hpp"


// Main object
class Board {
 private:
    // Lengthes of field (with side cells)
    static const int height = 100;
    static const int width  = 100;
    // Main array of cells
    Cell cells[height*width];
    // Main array of chanels between cells
    Channel verChanels[(height+1)*width];
    Channel upLeftChanels[height*width];  // ! Fix
    Channel downLeftChanels[height*width];  // ! Fix

 public:
    Board();
    void reset();
    int getWidth() const;
    int getHeight() const;
    bool isLegimate(SDL_Point pos) const;

    void update();

    const Cell& operator[](SDL_Point point) const;
};
