/*
 * Copyright (C) 2025-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#pragma once

#include "gase.hpp"


//
class Cell {
 protected:
    // ! Add walls

    // Physic part
    Gase gase;

 public:
    Cell();
    void reset();
    void rotate();

    // Interactions
    void applyMass(float koef);
    void reduceMass(float koef);
    void applyTemperature(float power);
    float getPressure() const;
    float getTemperature() const;

    // Every cycle update
    void exchange();  // Exchange with enviroment
    void exchange(Cell& other);  // Interact between 2 cell
    void vent(Cell& in, Cell& out) const;
    void exchangeValved(Cell& in, Cell& out) const;
    void cool(Cell& in, Cell& out) const;
    void applyChanges();

    // Drawing
    void blitNormal(const Window& window, SDL_FRect rect) const;
    void blitThermal(const Window& window, SDL_FRect rect) const;
    void blitPressure(const Window& window, SDL_FRect rect) const;
};
