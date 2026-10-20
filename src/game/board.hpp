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
    // Side cells not using
    Cell* cells;
    // Main array of chanels between cells
    Channel* downChanels;
    Channel* upRightChannels;
    Channel* downRightChannels;

    // Calculations per unit
    void projectUnit(Channel& bottom, Channel& upper, Channel& upRight, Channel& downLeft,
        Channel& downRight, Channel& upperLeft);
    void blitUnit(const Window& window, SDL_FRect rect, SDL_FColor color,
        bool down, bool upRight, bool downRight) const;
    //void blitWalls(const Window& window, SDL_FRect rect, bool down, bool upRight, bool downRight) const;

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
    void advect();

    // Draw
    void blitBlockNormal(const Window& window, SDL_FRect rect, int offset) const;
    void blitBlockThermal(const Window& window, SDL_FRect rect, int offset) const;
    void blitBlockPressure(const Window& window, SDL_FRect rect, int offset) const;
    void blitBlockRandom(const Window& window, SDL_FRect rect, int offset) const;
};
