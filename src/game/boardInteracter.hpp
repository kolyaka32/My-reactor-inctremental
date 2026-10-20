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

    // Interacting with scale and position
    void reset();
    void update();
    void checkNext();

    // Interaction
    Grid grid;
    // ! change to own class
    Cell holdingCell;
    SDL_FRect holdingCellRect;

    void setShowState(unsigned state);

    void blit() const;
};
