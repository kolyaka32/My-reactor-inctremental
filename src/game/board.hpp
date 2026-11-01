/*
 * Copyright (C) 2025-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#pragma once

#include "cell.hpp"
#include "channel.hpp"


// Additional info about one cell for show
struct CellInfo {
    // For cell
    float temperature;
    float pressure;
    // For channel
    float speed;
};

// Special position for channel
struct ChannelPos {
    int type;  // 0 for cell, 1 for up, 2 for leftUp and 3 for leftDown
    int x, y;
};

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
    // Draw info
    const SDL_FColor wallColor;

    Cell& getCell(int x, int y);
    Channel& getUpChannel(int x, int y);
    Channel& getLeftUpChannel(int x, int y);
    Channel& getLeftDownChannel(int x, int y);

    // Calculations per unit
    const float resistance = 0.4;
    void projectUnit(Cell& cell, Channel& upper, Channel& bottom, Channel& upperLeft,
        Channel& downRight, Channel& downLeft, Channel& upRight);
    void advectMass(Cell& cell1, Cell& cell2, Channel& channel);
    void advectSpeed(Channel& channel1, Channel& channel2);

    // Additional draw
    SDL_FColor getSpeedColor(SDL_Point pos) const;
    SDL_FColor getRandomColor() const;
    void blitUnit(const Window& window, SDL_FRect rect, SDL_FColor color) const;
    void blitWalls(const Window& window, SDL_FRect rect, bool upWall, bool upLeftWall,
        bool downLeftWall) const;
    void blitDown(const Window& window, SDL_FRect rect, bool wall) const;
    void blitUpperRight(const Window& window, SDL_FRect rect, bool wall) const;
    void blitDownRight(const Window& window, SDL_FRect rect, bool wall) const;

 public:
    Board(unsigned width, unsigned height) noexcept;
    Board(const Board& board) = delete;
    Board(Board&& board) = delete;
    ~Board() noexcept;
    void reset();

    // Interactions
    bool isLegimate(ChannelPos pos) const;
    CellInfo getInfo(ChannelPos pos) const;
    void applyMovement(ChannelPos pos, float val);
    void applyAction(ChannelPos pos);

    // Calculations
    void applyForces();
    void project();
    void advectMass();
    void advectSpeed();

    // Draw
    void blitBlockNormal(const Window& window, SDL_FRect rect, int offset) const;
    void blitBlockThermal(const Window& window, SDL_FRect rect, int offset) const;
    void blitBlockPressure(const Window& window, SDL_FRect rect, int offset) const;
    void blitBlockSpeed(const Window& window, SDL_FRect rect, int offset) const;
    void blitBlockRandom(const Window& window, SDL_FRect rect, int offset) const;
    void blitLines(const Window& window, SDL_FRect oddRect, SDL_FRect evenRect) const;
};
