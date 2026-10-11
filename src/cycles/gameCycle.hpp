/*
 * Copyright (C) 2025-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#pragma once

#include "baseCycle.hpp"
#include "../game/boardInteracter.hpp"


// Cycle with game
class GameCycle : public BaseCycle {
 protected:
    // Active game part
    BoardInteracter board;

    // New overrided cycle functions
    bool inputMouseDown() override;
    void inputMouseUp() override;
    void inputMouseWheel(float wheelY) override;
    void inputKeys(SDL_Keycode key) override;
    void update() override;
    void draw() const override;

 public:
    GameCycle(Window& _window);
};
