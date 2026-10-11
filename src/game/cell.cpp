/*
 * Copyright (C) 2025-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "cell.hpp"


Cell::Cell() {}

void Cell::reset() {
    state = Air;
    gase.reset();
}

bool Cell::isWall() const {
    return state & WALL_MASK;
}

bool Cell::isRotable() const {
    return state & ROTATE_MASK;
}

void Cell::rotate() {
    // Check, if part rotatable
    if (isRotable()) {
        // Get new rotation
        CellState rotation = (state & ANGLE_MASK) + 1;
        // Set it to state
        state = (state & ~ANGLE_MASK) | (rotation & ANGLE_MASK);
    }
}

void Cell::applyMass(float _koef) {
    if (!isWall()) {
        gase.addMass(_koef, Gase::environment);
    }
}

void Cell::reduceMass(float _koef) {
    if (!isWall()) {
        gase.reduceMass(_koef);
    }
}

void Cell::applyTemperature(float _power) {
    if (!isWall()) {
        gase.addTemperature(_power);
    }
}

float Cell::getPressure() const {
    return gase.getPressure();
}

float Cell::getTemperature() const {
    return gase.getTemperature();
}

void Cell::exchange() {
    if (!isWall()) {
        gase.exchange();
    }
}

void Cell::exchange(Cell& _other) {
    if (!isWall() && !_other.isWall()) {
        gase.exchange(_other.gase);
    }
}

void Cell::vent(Cell& _in, Cell& _out) const {
    if (!_in.isWall() && !_out.isWall()) {
        _in.gase.vent(_out.gase, 2.0);
    }
}

void Cell::exchangeValved(Cell& _in, Cell& _out) const {
    if (!_in.isWall() && !_out.isWall()) {
        _in.gase.exchangeValved(_out.gase);
    }
}

void Cell::cool(Cell& _in, Cell& _out) const {
    if (!_in.isWall() && !_out.isWall()) {
        _in.gase.cool(_out.gase, 40.0);
    }
}

void Cell::applyChanges() {
    gase.applyChanges();
}

void Cell::blitNormal(const Window& _window, SDL_FRect _rect) const {
    switch (state) {
    case Air:
        _window.setDrawColor(WHITE);
        _window.drawRect(_rect);
        break;

    case Heater:
        _window.blit(_window.getTexture(Textures::Heater), _rect);
        break;

    case Wall:
        _window.blit(_window.getTexture(Textures::Wall), _rect);
        break;

    case VentUp:
        _window.blit(_window.getTexture(Textures::Vent), _rect);
        break;

    case VentRight:
        _window.blit(_window.getTexture(Textures::Vent), 90.0, _rect);
        break;

    case VentDown:
        _window.blit(_window.getTexture(Textures::Vent), 180.0, _rect);
        break;

    case VentLeft:
        _window.blit(_window.getTexture(Textures::Vent), 270.0, _rect);
        break;

    case ValveUp:
        _window.blit(_window.getTexture(Textures::Valve), _rect);
        break;

    case ValveRight:
        _window.blit(_window.getTexture(Textures::Valve), 90.0, _rect);
        break;

    case ValveDown:
        _window.blit(_window.getTexture(Textures::Valve), 180.0, _rect);
        break;

    case ValveLeft:
        _window.blit(_window.getTexture(Textures::Valve), 270.0, _rect);
        break;

    case CoolerUp:
        _window.blit(_window.getTexture(Textures::Cooler), _rect);
        break;

    case CoolerRight:
        _window.blit(_window.getTexture(Textures::Cooler), 90.0, _rect);
        break;

    case CoolerDown:
        _window.blit(_window.getTexture(Textures::Cooler), 180.0, _rect);
        break;

    case CoolerLeft:
        _window.blit(_window.getTexture(Textures::Cooler), 270.0, _rect);
        break;

    case Buldozer:
        _window.blit(_window.getTexture(Textures::Buldozer), _rect);
        break;

    default:
        break;
    }
}

void Cell::blitThermal(const Window& _window, SDL_FRect _rect) const {
    // Check, if cell can has pressure
    if (isWall()) {
        blitNormal(_window, _rect);
    } else {
        gase.blitThermal(_window, _rect);
    }
}

void Cell::blitPressure(const Window& _window, SDL_FRect _rect) const {
    // Check, if cell can has pressure
    if (isWall()) {
        blitNormal(_window, _rect);
    } else {
        gase.blitPressure(_window, _rect);
    }
}
