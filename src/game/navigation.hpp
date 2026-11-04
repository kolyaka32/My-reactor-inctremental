/*
 * Copyright (C) 2025, Kazankov Nikolay 
 * <nik.kazankov.05@mail.ru>
 */

#pragma once

#include "../data/app.hpp"


//
class Grid {
 private:
    // Parameters of grid system
    float centerX, centerY;
    float scale;

    // Parameters of movement
    float captureX, captureY;

 public:
    Grid();
    void reset();

    void click(const Mouse mouse);
    void zoom(float wheelY, const Mouse mouse);
    void update(const Mouse mouse);

    // Work with local and absolute coordinats
    float absoluteX(float localX) const;
    float absoluteY(float localY) const;
    SDL_FPoint absolute(SDL_FPoint local) const;
    SDL_FRect absolute(SDL_FRect local) const;
    float localX(float X) const;
    float localY(float Y) const;
    SDL_FPoint local(SDL_FPoint absolute) const;
    SDL_FPoint local(const Mouse mouse) const;
    SDL_FRect local(SDL_FRect absolute) const;
};
