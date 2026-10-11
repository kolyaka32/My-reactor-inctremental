/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "gase.hpp"


// Default world parameters
Gase Gase::environment{100.0, 300.0};

Gase::Gase() {}

Gase::Gase(float _pressure, float _temperature) {
    temperature = _temperature;
    mass = _pressure * volume / _temperature;
    // Setting temporary variables to basic
    newMass = mass;
    newEnergy = mass * temperature * heatCapacity;
}

void Gase::reset() {
    // Setting parameters to global
    temperature = environment.temperature;
    mass = environment.mass;
    // Setting temporary variables to basic
    newMass = mass;
    newEnergy = mass * temperature * heatCapacity;
}

float Gase::getPressure() const {
    return mass * temperature / volume;
}

float Gase::getMass() const {
    return mass;
}

float Gase::getTemperature() const {
    return temperature;
}

void Gase::reduceMass(float _change) {
    // Temperature doesn't change
    newMass /= _change;
    newEnergy /= _change;
}

void Gase::addTemperature(float _power) {
    // Mass stay constant
    newEnergy += _power;
}

float Gase::getMassFlow(const Gase& _second, float _power) const {
    float d = (getPressure() - _second.getPressure() + _power) * pressureKoef;
    // Check, if exceed mass
    if (d > mass/8) {
        return mass/8;
    }
    if (d < -_second.mass/8) {
        return -_second.mass/8;
    }
    return d;
}

void Gase::addMass(float _deltaMass, const Gase _srcGase) {
    // Changing energy (temperature as result)
    newEnergy += _deltaMass * _srcGase.temperature * _srcGase.heatCapacity;
    // Changing mass
    newMass += _deltaMass;
}

void Gase::exchange() {
    // Getting change
    float deltaMass = getMassFlow(environment);

    // Changing temperture (not affecting enviroment)
    if (deltaMass > 0) {
        addMass(-deltaMass, *this);
    } else {
        addMass(-deltaMass, environment);
    }
}

void Gase::exchange(Gase& _other) {
    // Getting change
    float deltaMass = getMassFlow(_other);

    // Changing temperture
    if (deltaMass > 0) {
        addMass(-deltaMass, *this);
        _other.addMass(deltaMass, *this);
    } else {
        addMass(-deltaMass, _other);
        _other.addMass(deltaMass, _other);
    }
}

void Gase::vent(Gase& _outGase, float _power) {
    // Getting change
    float deltaMass = getMassFlow(_outGase, _power);

    // Changing temperture
    if (deltaMass > 0) {
        addMass(-deltaMass, *this);
        _outGase.addMass(deltaMass, *this);
    } else {
        addMass(-deltaMass, _outGase);
        _outGase.addMass(deltaMass, _outGase);
    }
}

void Gase::exchangeValved(Gase& _outGase) {
    // Getting change
    float deltaMass = getMassFlow(_outGase);

    // Allow only to one side
    if (deltaMass > 0) {
        // Changing temperture
        addMass(-deltaMass, *this);
        _outGase.addMass(deltaMass, *this);
    }
}

void Gase::cool(Gase& _outGase, float _power) {
    float delta = (temperature - _outGase.temperature - _power) * 0.5;

    if (temperature > delta) {
        // Exchanging energy
        newEnergy -= delta * mass * heatCapacity;
        _outGase.newEnergy += delta * _outGase.mass * heatCapacity;
    }
}

void Gase::applyChanges() {
    mass = newMass;
    temperature = newEnergy / mass / heatCapacity;
}

void Gase::blitThermal(const Window& _window, SDL_FRect _rect) const {
    if (temperature < 300.0) {
        // Cold spectre
        _window.setDrawColor({0, 0, Uint8(255 - temperature*(255.0/300.0)), 255});
    } else if (temperature < 300.0 + 255.0) {
        // Warm spectre
        _window.setDrawColor({Uint8(temperature-300.0), 0, 0, 255});
    } else {
        // Over hotx
        _window.setDrawColor({255, 0, 0, 255});
    }
    _window.drawRect(_rect);
}

void Gase::blitPressure(const Window& _window, SDL_FRect _rect) const {
    float pressure = getPressure();

    // Doesn't check less then 0
    if (pressure > 255/drawPressureKoef) {
        _window.setDrawColor({0, 0, 255, 255});
    } else {
        _window.setDrawColor({0, 0, Uint8(pressure*drawPressureKoef), 255});
    }
    _window.drawRect(_rect);
}
