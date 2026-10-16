/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#pragma once

#include "../GUI/baseGUI.hpp"
#include "board.hpp"
#include "navigation.hpp"
#include "cellTexture.hpp"


// Possible states of current showing
enum class ShowState {
    Normal,
    Thermal,
    Pressure,
};

// Interact tool with board
class BoardInteracter : public GUI::Template {
private:
    Board board;
    ShowState showState;

    // Rects of local positions of board
    const SDL_FRect cellRect;

    // ! Check, if required
    CellTexture texture;

    // Draw different modes/parts
    void blitLines() const;

public:
    BoardInteracter(const Window& window);

    // Interacting with scale and position
    void reset();
    void update();

    // Interaction
    // ! change to own class
    Cell holdingCell;
    SDL_FRect holdingCellRect;

    void setShowState(unsigned state);

    void blit() const;
};
