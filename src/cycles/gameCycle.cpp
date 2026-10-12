/*
 * Copyright (C) 2025-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "gameCycle.hpp"


GameCycle::GameCycle(Window& _window)
: BaseCycle(_window),
board(_window, 0.26, 0.05, 0.25) {
    if (!isRestarted()) {
        // Resetting field
        board.reset();
    }
    logger.additional("Start game cycle");
}

bool GameCycle::inputMouseDown() {
    if (BaseCycle::inputMouseDown()) {
        return true;
    }
    board.click(mouse);
    return false;
}

void GameCycle::inputMouseUp() {
    BaseCycle::inputMouseUp();
    board.unclick(mouse);
}

bool GameCycle::inputMouseWheel(float _wheelY) {
    if (BaseCycle::inputMouseWheel(_wheelY)) {
        return true;
    }
    if (board.scroll(mouse, _wheelY)) {
        return true;
    }
    return false;
}

bool GameCycle::inputKeys(SDL_Keycode _key) {
    if (board.press(_key)) {
        return true;
    }
    if (BaseCycle::inputKeys(_key)) {
        return true;
    }
    return false;
}

void GameCycle::update() {
    mouse.updatePos();
    BaseCycle::update();
    board.update(mouse);
}

void GameCycle::draw() const {
    // Bliting background
    window.setDrawColor(GREY);
    window.clear();

    // Blitting field
    board.blit();

    // Drawing upper dashboard
    settings.blit();

    // Bliting all to screen
    window.render();
}
