/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#pragma once

#include "../data/app.hpp"


// Class for interact with gase state and exchange
class Gase {
private:
    // Parameters of one cell of gase
    // Required 3 from 4 paramters: mass, presure, temperature, volume
    // Volume is constant and = 1
    float mass;         // [kg]
    float temperature;  // [K]
    // Inter-cycle variables for temporary storage
    float newMass;      // [kg]
    float newEnergy;    // [J]

    // Constants
    static constexpr float volume = 1.0;         // [m^3]
    static constexpr float pressureKoef = 0.01;  // [1]
    static constexpr float heatCapacity = 1.0;   // [J/kg/K]
    static constexpr float drawTemperatureKoef = 2.5;
    static constexpr float drawPressureKoef = 1.0;

    float getMassFlow(const Gase& second, float power = 0.0) const;

public:
    // Environment parameters
    static Gase environment;

    Gase();
    Gase(float pressure, float temperature);
    void reset();

    // Getters
    float getPressure() const;
    float getMass() const;
    float getTemperature() const;

    // Interactions
    void reduceMass(float koefMass);
    void addMass(float deltaMass, const Gase srcGase);
    void addTemperature(float power);

    // Every cycle updates
    // Exchange of heat and pressure between cell with saving to new place
    void exchange();  // Exchange with enviroment
    void exchange(Gase& other);  // Exchange between two elements
    void vent(Gase& outGase, float power);  // Take air from current cell to outlet
    void exchangeValved(Gase& outGase);  // Allow flow only to one direction
    void cool(Gase& outGase, float power);  // Cool tile by heating up out tile
    void applyChanges();  // Apply accumulated per exchanges changes

    // Drawing
    void blitThermal(const Window& window, SDL_FRect rect) const;
    void blitPressure(const Window& window, SDL_FRect rect) const;
};
