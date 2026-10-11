/*
 * Copyright (C) 2025, Kazankov Nikolay 
 * <nik.kazankov.05@mail.ru>
 */

#include "navigation.hpp"


// Static objects
Grid grid;

Grid::Grid() {
    reset();
}

void Grid::reset() {
    centerX = 0;
    centerY = 0;
    scale = 1;
}

void Grid::update(float mouseX, float mouseY) {
    // Keeping captured point at it place
    centerX = mouseX - captureX*scale;
    centerY = mouseY - captureY*scale;
    // logger.additional("Press at %f : %f, get at %f : %f", mouseX, mouseY, centerX, centerY);
}

void Grid::click(float mouseX, float mouseY) {
    captureX = (mouseX - centerX)/scale;
    captureY = (mouseY - centerY)/scale;
    logger.additional("Press at %f : %f, get at %f : %f", mouseX, mouseY, captureX, captureY);
}

void Grid::zoom(float _wheelY, const Mouse _mouse) {
    centerX = (_mouse.getX()-centerX) / scale;
    centerY = (_mouse.getY()-centerY) / scale;

    logger.additional("Scaling center: %f,%f scale:%f", centerX, centerY, scale);

    scale *= SDL_powf(1.5, _wheelY);

    setMin(scale, 0.1f);
    setMax(scale, 20.0f);

    centerX = _mouse.getX() - centerX * scale;
    centerY = _mouse.getY() - centerY * scale;

    logger.additional("New center: %f,%f scale:%f", centerX, centerY, scale);
}

float Grid::absoluteX(float _x) const {
    return _x * scale + centerX;
}

float Grid::absoluteY(float _y) const {
    return _y * scale + centerY;
}

SDL_FPoint Grid::absolute(const SDL_FPoint _local) const {
    return SDL_FPoint{absoluteX(_local.x),
        absoluteY(_local.y)};
}

SDL_FRect Grid::absolute(SDL_FRect _local) const {
    return SDL_FRect{absoluteX(_local.x),
        absoluteY(_local.y),
        _local.w*scale,
        _local.h*scale};
}

float Grid::localX(float _x) const {
    return (_x - centerX) / scale;
}

float Grid::localY(float _y) const {
    return (_y - centerY) / scale;
}

SDL_FPoint Grid::local(SDL_FPoint _absolute) const {
    return SDL_FPoint{localX(_absolute.x),
        localX(_absolute.y)};
}

SDL_FPoint Grid::local(const Mouse _mouse) const {
    return SDL_FPoint{localX(_mouse.getX()),
        localY(_mouse.getY())};
}

SDL_FRect Grid::local(SDL_FRect _absolute) const {
    return SDL_FRect{localX(_absolute.x),
        localY(_absolute.y),
        _absolute.w/scale,
        _absolute.h/scale};
}
