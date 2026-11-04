/*
 * Copyright (C) 2025-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "gameCycle.hpp"


GameCycle::GameCycle(Window& _window)
: BaseCycle(_window),
board(_window),
menu(_window, 0.125, 0.25, board) {
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
    menu.click(mouse);
    return false;
}

void GameCycle::inputMouseUp() {
    BaseCycle::inputMouseUp();
    menu.unclick(mouse);
}

bool GameCycle::inputMouseWheel(float _wheelY) {
    if (BaseCycle::inputMouseWheel(_wheelY)) {
        return true;
    }
    if (menu.scroll(mouse, _wheelY)) {
        return true;
    }
    return false;
}

bool GameCycle::inputKeys(SDL_Keycode _key) {
    if (menu.press(_key)) {
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
    menu.update(mouse);
    board.update();
}

void GameCycle::draw() const {
    // Bliting background
    window.setDrawColor(GREY);
    window.clear();

    // Blitting field
    board.blit();

    menu.blit();

    // Drawing upper dashboard
    settings.blit();

    // Bliting all to screen
    window.render();
}
