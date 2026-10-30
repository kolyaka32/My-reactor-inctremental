/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#pragma once

#include "../GUI/interface.hpp"
#include "boardInteracter.hpp"


// Menu for interacting with board
class ControlMenu : public GUI::Template {
 protected:
    // Key element, interact with
    BoardInteracter& interacter;

    // State for pressing mouse from SDL_MouseButtonFlags
    int mousePress;
    // Current selected object
    ChannelPos pos;

    // Elements
    GUI::RectBackplate panelBackplate;
    GUI::StaticText modeText;
    GUI::SwitchBox modeSwitchBox;
    GUI::DynamicText pickedElement;
    GUI::DynamicText pickedPressure, pickedTemperature;
    GUI::DynamicText pickedUpSpeed, pickedLeftUpSpeed, pickedLeftDownSpeed;
    GUI::StaticText buildText;
    GUI::SwitchBox buildSwitchBox;
    GUI::TextButton resetButton;
    GUI::StaticText stoppedText;

 public:
    ControlMenu(const Window& window, float boardX, float boardY, float panelW, BoardInteracter& interacter);

    void reset();
    bool click(const Mouse mouse);
    void unclick(const Mouse mouse);
    bool scroll(const Mouse mouse, float wheelY);
    bool press(SDL_Keycode key);
    void update(const Mouse mouse);
    void blit() const;
};
