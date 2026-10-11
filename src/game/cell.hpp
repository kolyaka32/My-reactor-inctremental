/*
 * Copyright (C) 2025-2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#pragma once

#include "gase.hpp"


//
class Cell {
 protected:
    // Masks for state interaction
    typedef Uint8 CellState;  // Type for storing cell state

    // State defines as 1 bit for air throwput, 3 bits for type, 2 bits for rotation
    // Avaliable gase state
    static const CellState WALL_MASK = 0b1000000;  // Represent, is cell contains gase
    static const CellState GASE = 0b0000000;
    static const CellState WALL = 0b1000000;

    // States
    static const CellState TYPE_MASK = 0b0111000;  // Represent cell type

    // Avaliable angles
    static const CellState ROTATE_MASK = 0b0000100;  // Represent, if cell could be rotate
    static const CellState ANGLE_MASK  = 0b0000011;  // Represent current rotation of cell
    static const CellState UP    = 0b100;
    static const CellState RIGHT = 0b101;
    static const CellState DOWN  = 0b110;
    static const CellState LEFT  = 0b111;

    // Check state
    bool isWall() const;  // Return, if air can't came throw
    bool isRotable() const;  // Return, if part can be rotate

    // Variables per cell
    Gase gase;

 public:
    // Possible states of cell
    enum State : CellState {
        // Hollow, air can came
        Air         = GASE | 0b000 << 3,
        Heater      = GASE | 0b001 << 3,

        // Filled, air can't came throw
        Wall        = WALL | 0b000 << 3,
        // Vent object
        VentUp      = WALL | 0b001 << 3 | UP,
        VentRight   = WALL | 0b001 << 3 | RIGHT,
        VentDown    = WALL | 0b001 << 3 | DOWN,
        VentLeft    = WALL | 0b001 << 3 | LEFT,
        // Check valve (allow flow to one side)
        ValveUp     = WALL | 0b010 << 3 | UP,
        ValveRight  = WALL | 0b010 << 3 | RIGHT,
        ValveDown   = WALL | 0b010 << 3 | DOWN,
        ValveLeft   = WALL | 0b010 << 3 | LEFT,
        // Coolers (cool one side and heat another)
        CoolerUp    = WALL | 0b011 << 3 | UP,
        CoolerRight = WALL | 0b011 << 3 | RIGHT,
        CoolerDown  = WALL | 0b011 << 3 | DOWN,
        CoolerLeft  = WALL | 0b011 << 3 | LEFT,

        // Special
        Buldozer    = 0b11111111,
    };

    // Represent object (from State)
    Uint8 state = Air;

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
