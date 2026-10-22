/*
 * Copyright (C) 2026, Kazankov Nikolay
 * <nik.kazankov.05@mail.ru>
 */

#include "cell.hpp"


Cell::Cell() {}

/*Cell::Cell(float _pressure, float _temperature) {
    temperature = _temperature;
    mass = _pressure * volume / _temperature;
    // Setting temporary variables to basic
    newMass = mass;
    newEnergy = mass * temperature * heatCapacity;
}*/

void Cell::reset() {
    // Setting parameters to global
    //temperature = environment.temperature;
    //mass = environment.mass;
    //temperature = SDL_randf()*256+300;
    static float a = 0.0;
    temperature = a;
    a += 5.0;
    pressure = 0.0;
    //mass = 1.0;

    // Setting temporary variables to basic
    //newMass = mass;
    //newEnergy = mass * temperature * heatCapacity;
}

/*float Cell::getPressure() const {
    return mass * temperature / volume;
}

float Cell::getMass() const {
    return mass;
}

float Cell::getTemperature() const {
    return temperature;
}

void Cell::reduceMass(float _change) {
    // Temperature doesn't change
    newMass /= _change;
    newEnergy /= _change;
}

void Cell::addTemperature(float _power) {
    // Mass stay constant
    newEnergy += _power;
}

float Cell::getMassFlow(const Cell& _second, float _power) const {
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

void Cell::addMass(float _deltaMass, const Cell _srcGase) {
    // Changing energy (temperature as result)
    newEnergy += _deltaMass * _srcGase.temperature * _srcGase.heatCapacity;
    // Changing mass
    newMass += _deltaMass;
}

void Cell::exchange(Cell& _other) {
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

void Cell::vent(Cell& _outGase, float _power) {
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

void Cell::exchangeValved(Cell& _outGase) {
    // Getting change
    float deltaMass = getMassFlow(_outGase);

    // Allow only to one side
    if (deltaMass > 0) {
        // Changing temperture
        addMass(-deltaMass, *this);
        _outGase.addMass(deltaMass, *this);
    }
}

void Cell::cool(Cell& _outGase, float _power) {
    float delta = (temperature - _outGase.temperature - _power) * 0.5;

    if (temperature > delta) {
        // Exchanging energy
        newEnergy -= delta * mass * heatCapacity;
        _outGase.newEnergy += delta * _outGase.mass * heatCapacity;
    }
}

void Cell::applyChanges() {
    mass = newMass;
    temperature = newEnergy / mass / heatCapacity;
}*/

SDL_FColor Cell::getNormalColor() const {
    // Normal color map
    return {0.6, 0.6, 0.6, 1.0};
}

SDL_FColor Cell::getThermalColor() const {
    constexpr float drawTemperatureKoef = 256.0;

    if (temperature < 300.0) {
        // Cold spectre
        return {0.0, 0.0, 1.0f - temperature/300.0f, 1.0};
    } else if (temperature < 300.0 + drawTemperatureKoef) {
        // Warm spectre
        return {(temperature-300.0f)/drawTemperatureKoef, 0.0, 0.0, 1.0};
    }
    // Too hot
    return {1.0, 0.0, 0.0, 1.0};
}

SDL_FColor Cell::getPressureColor() const {
    //float pressure = getPressure();
    constexpr float drawPressureKoef = 1.0;
    if (pressure > 0.0) {
        // If greater - red
        if (pressure > 1.0/drawPressureKoef) {
            return {1.0, 0.0, 0.0, 1.0};
        }
        return {pressure*drawPressureKoef, 0.0, 0.0, 1.0};
    }
    // If less - blue
    if (pressure < -1.0/drawPressureKoef) {
        return {0.0, 0.0, 1.0, 1.0};
    }
    return {0.0, 0.0, -pressure*drawPressureKoef, 1.0};
}
