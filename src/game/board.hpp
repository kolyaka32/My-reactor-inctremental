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
    const int height;
    const int width;
    // Main array of cells
    Cell* cells;
    // Main array of chanels between cells
    Channel* verChanels;
    Channel* upLeftChanels;
    Channel* downLeftChanels;

 public:
    Board(unsigned width, unsigned height) noexcept;
    Board(const Board& board) = delete;
    Board(Board&& board) = delete;
    ~Board() noexcept;

    // Interaction
    void reset();
    int getWidth() const;
    int getHeight() const;
    bool isLegimate(SDL_Point pos) const;
    const Cell& operator[](SDL_Point point) const;
    // Calculations
    void applyForces();
    void project();
    void advect();  // Actual fluid transfer
};
