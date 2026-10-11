/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#pragma once

#include "../GUI/interface.hpp"
#include "navigation.hpp"
#include "board.hpp"


// Possible states of current showing
enum class ShowState {
    Normal,
    Thermal,
    Pressure,
};

// Class for interact with and visualise board
class BoardInteracter : GUI::Template {
private:
    static Board board;
    static ShowState showState;
    int mousePress;  // State for pressing mouse from SDL_MouseButtonFlags

    // Rects of local positions of board
    const SDL_FRect cellRect;

    // Selected object
    Cell holdingCell;
    SDL_FRect holdingCellRect;

    // Graphical part
    GUI::RectBackplate panelBackplate;
    GUI::HighlightedStaticText modeText;
    GUI::SwitchBox modeSwitchBox;
    GUI::DynamicText pickedPressure;
    GUI::DynamicText pickedTemperature;
    GUI::HighlightedStaticText buildText;
    GUI::SwitchBox buildSwitchBox;
    GUI::TextButton resetButton;

public:
    BoardInteracter(const Window& window, float boardX, float boardY, float panelW);

    void reset();

    // Interacting with scale and position
    void click(const Mouse mouse);
    void update(const Mouse mouse);
    void unclick(const Mouse mouse);
    void scroll(const Mouse mouse, float wheelY);
    bool press(SDL_Keycode key);

    void blit() const;
};
