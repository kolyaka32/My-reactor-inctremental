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
    mouse.updatePos();
    board.unclick(mouse);
    settings.unClick();
}

void GameCycle::inputMouseWheel(float _wheelY) {
    mouse.updatePos();
    BaseCycle::inputMouseWheel(_wheelY);
    board.scroll(mouse, _wheelY);
}

void GameCycle::inputKeys(SDL_Keycode _key) {
    if (board.press(_key)) {
        return;
    }
    if (_key == SDLK_ESCAPE) {
        // Closing top open object
        settings.activate();
        return;
    }
}

void GameCycle::update() {
    BaseCycle::update();

    mouse.updatePos();

    board.update(mouse);
}

void GameCycle::draw() const {
    // Bliting background
    window.setDrawColor(GREY);
    window.clear();

    // Blitting field
    board.blit();

    // Drawing upper dashboard
    exitButton.blit();
    settings.blit();

    // Bliting all to screen
    window.render();
}
