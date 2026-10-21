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
    Channel* upChannels;  // End on extra line
    Channel* upLeftChannels;  // End on extra line
    Channel* downLeftChannels;  // Starting on extra upper line

    // Calculations per unit
    void projectUnit(Cell& cell, Channel& upper, Channel& bottom, Channel& upperLeft,
        Channel& downRight, Channel& downLeft, Channel& upRight);

    // Additional draw
    void blitUnit(const Window& window, SDL_FRect rect, SDL_FColor color,
        bool upWall, bool upLeftWall, bool downLeftWall) const;
    void blitDown(const Window& window, SDL_FRect rect) const;
    void blitUpperRight(const Window& window, SDL_FRect rect) const;
    void blitDownRight(const Window& window, SDL_FRect rect) const;

 public:
    Board(unsigned width, unsigned height) noexcept;
    Board(const Board& board) = delete;
    Board(Board&& board) = delete;
    ~Board() noexcept;

    // Interaction
    void reset();
    bool isLegimate(SDL_Point pos) const;
    float getTemperature(SDL_Point pos) const;
    float getPressure(SDL_Point pos) const;
    float getSpeed(SDL_Point pos) const;

    // Calculations
    void applyForces();
    void project();
    void advect();

    // Draw
    void blitBlockNormal(const Window& window, SDL_FRect rect, int offset) const;
    void blitBlockThermal(const Window& window, SDL_FRect rect, int offset) const;
    void blitBlockPressure(const Window& window, SDL_FRect rect, int offset) const;
    void blitBlockRandom(const Window& window, SDL_FRect rect, int offset) const;
    void blitLines(const Window& window, SDL_FRect oddRect, SDL_FRect evenRect) const;
};
