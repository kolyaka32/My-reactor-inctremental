/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#pragma once

#include "../GUI/baseGUI.hpp"
#include "board.hpp"
#include "navigation.hpp"


// Possible states of current showing
enum class ShowState {
    Normal,
    Thermal,
    Pressure,
    Speed,
    Random,
};

// Interact tool with board
class BoardInteracter : public GUI::Template {
private:
    Board board;
    ShowState showState;

    // Draw rects in local positions
    SDL_FRect oddRect, evenRect;

public:
    BoardInteracter(const Window& window);

    void reset();
    void setShowState(unsigned state);
    SDL_Point getPosition(const Mouse mouse) const;
    bool isLegimate(SDL_Point pos) const;
    CellInfo getCellInfo(SDL_Point pos) const;
    void applyMovement(SDL_Point pos, float val);

    // Cycle
    void update();
    void blit() const;

    // Interaction
    Grid grid;

    // ! change to own class
    Cell holdingCell;
    SDL_FRect holdingCellRect;
};
